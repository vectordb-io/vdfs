#ifndef __VDFS_H__
#define __VDFS_H__

#include <cstdio>
#include <set>
#include <map>
#include <vector>
#include <string>
#include <iostream>
#include <leveldb/db.h>
#include "vdfs.pb.h"

namespace vectordb {

class Vdfs {

#define BLOCK_SIZE 32

  public:
    Vdfs(const std::string& path);
    ~Vdfs();
    Vdfs(const Vdfs&) = delete;
    Vdfs& operator=(const Vdfs&) = delete;

    std::string meta_path() const {
        return path_ + "/meta";
    }

    std::string data_path() const {
        return path_ + "/data";
    }

    bool Load();
    bool Create();
    bool Put(const std::string &file_path) const;
    bool Cat(const std::string &file_name) const;

  private:
    std::string path_;
    leveldb::DB* db_meta_;
    leveldb::DB* db_data_;
};

} // namespace vectordb

#endif
