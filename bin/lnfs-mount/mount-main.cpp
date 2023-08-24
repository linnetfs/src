/* Copyright (c) Jeremías Casteglione <jrmsdev@gmail.com> */
/* See LICENSE file. */

// https://libfuse.github.io/doxygen/passthrough_8c_source.html

#include "lnfs.h"

static const struct fuse_operations lnfs_operations = {};

int main(int argc, char *argv[])
{
	lnfs_debug("main");
	umask(0);
	return fuse_main(argc, argv, &lnfs_operations, NULL);
}
