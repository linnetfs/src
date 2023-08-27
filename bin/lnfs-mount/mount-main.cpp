/* Copyright (c) Jeremías Casteglione <jrmsdev@gmail.com> */
/* See LICENSE file. */

#include "lnfs.hpp"
#include "lnfs_log.hpp"

#include <sys/stat.h> // for umask

int main(int argc, char *argv[])
{
	lnfs_debug("{}", lnfs_name());
	umask(0);
	return fuse_main(argc, argv, lnfs_operations(), NULL);
}
