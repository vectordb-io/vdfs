#include <iostream>
#include "vdfs.h"
#include "util.h"

int
main(int argc, char **argv) {
    vectordb::Vdfs vdfs("./test_vdfs");
    vdfs.Create();

    auto b = vdfs.Put("./Makefile.test");
    assert(b);
    b = vdfs.Cat("Makefile.test");
    assert(b);

    return 0;
}
