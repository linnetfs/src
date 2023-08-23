#include "lnfs.h"

static const struct fuse_operations lnfs_operations = {};

int main(int argc, char *argv[])
{
    umask(0);
    return fuse_main(argc, argv, &lnfs_operations, NULL);
}
