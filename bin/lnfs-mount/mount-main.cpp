/* Copyright (c) Jeremías Casteglione <jrmsdev@gmail.com> */
/* See LICENSE file. */

#include "lnfs.hpp"
#include "lnfs_log.hpp"

int main(int argc, char *argv[])
{
	lnfs_debug("{} {}", LNFS_NAME, LNFS_MODE);
	umask(0);
	return fuse_main(argc, argv, lnfs_operations(), NULL);
}
