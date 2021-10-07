#include <cstdio>
#include "util.h"
#include "vdfs.h"

namespace vectordb {

Vdfs::Vdfs(const std::string& path)
    :path_(path) {
}

Vdfs::~Vdfs() {
    delete db_meta_;
    delete db_data_;
}

bool
Vdfs::Load() {

    return true;
}

bool
Vdfs::Create() {
    auto b = util::RecurMakeDir2(path_);
    assert(b);

    leveldb::Options options;
    options.create_if_missing = true;
    options.error_if_exists = true;

    auto s = leveldb::DB::Open(options, meta_path(), &db_meta_);
    assert(s.ok());

    s = leveldb::DB::Open(options, data_path(), &db_data_);
    assert(s.ok());

    return true;
}

bool
Vdfs::Put(const std::string &file_path) const {
    std::string file_name;
    std::vector<std::string> sv;
    util::Split(file_path, '/', sv, "");
    file_name = *(sv.rbegin());

    int count = 0;
    FILE *fp = fopen(file_path.c_str(),"r");
    assert(fp);
    while (true) {
        char buf[BLOCK_SIZE];
        int n = fread(buf, 1, BLOCK_SIZE, fp);
        if (n > 0) {
            char key_buf[128];
            snprintf(key_buf, sizeof(key_buf), "%s_%d", file_name.c_str(), count);
            auto s = db_data_->Put(leveldb::WriteOptions(), std::string(key_buf), std::string(buf, n));
            assert(s.ok());
            count++;

        } else {
            break;
        }
    }
    fclose(fp);

    vdfs::Int32 pb;
    pb.set_data(count);
    std::string str;
    pb.SerializeToString(&str);
    auto s = db_meta_->Put(leveldb::WriteOptions(), file_name, str);
    assert(s.ok());

    return true;
}

bool
Vdfs::Cat(const std::string &file_name) const {
    std::string value;
    auto s = db_meta_->Get(leveldb::ReadOptions(), file_name, &value);
    assert(s.ok());

    vdfs::Int32 pb;
    pb.ParseFromString(value);
    int count = pb.data();

    std::string tmp_file;
    for (int i = 0; i < count; ++i) {
        std::string tmp_value;
        char key_buf[128];
        snprintf(key_buf, sizeof(key_buf), "%s_%d", file_name.c_str(), i);
        auto s = db_data_->Get(leveldb::ReadOptions(), std::string(key_buf), &tmp_value);
        assert(s.ok());

        tmp_file.append(tmp_value);
    }
    std::cout << tmp_file << std::endl;

    return true;
}

} // namespace vectordb
