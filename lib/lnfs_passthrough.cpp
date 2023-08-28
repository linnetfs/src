/* Copyright (c) Jeremías Casteglione <jrmsdev@gmail.com> */
/* See LICENSE file. */

#include "lnfs_passthrough.hpp"

#include <cerrno>
#include <unistd.h> // for: access

#include "lnfs_log.hpp"

//------------------------------------------------------------------------------

void* lnfs_init(fuse_conn_info* conn, fuse_config* cfg)
{
	lnfs_debug("passthrough init");
	(void) conn;
	cfg->use_ino = 1;
	cfg->entry_timeout = 0;
	cfg->attr_timeout = 0;
	cfg->negative_timeout = 0;
	return nullptr;
}

//------------------------------------------------------------------------------
// man 2 lstat

int lnfs_getattr(const char* path, struct stat* stbuf, fuse_file_info* fi)
{
	lnfs_debug("passthrough getattr {}", path);
	(void) fi;
	int res;
	res = lstat(path, stbuf);
	if (res == -1)
		return -errno;
	return 0;
}

//------------------------------------------------------------------------------
// man 2 access

int lnfs_access(const char* path, int mask)
{
	lnfs_debug("passthrough access {} {}", path, mask);
	int res;
	res = access(path, mask);
	if (res == -1)
		return -errno;
	return 0;
}

//------------------------------------------------------------------------------

int lnfs_readlink(const char* path, char* buf, size_t size)
{
	lnfs_debug("passthrough readlink {}", path);
	int res;
	res = readlink(path, buf, size - 1);
	if (res == -1)
		return -errno;
	buf[res] = '\0';
	return 0;
}

//------------------------------------------------------------------------------

static const fuse_operations ops = {
	.getattr  = lnfs_getattr,
	.readlink = lnfs_readlink,
	.init     = lnfs_init,
	.access   = lnfs_access,
};

const fuse_operations* lnfs_operations()
{
	lnfs_debug("passthrough operations");
	return &ops;
}
