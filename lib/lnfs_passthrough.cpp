/* Copyright (c) Jeremías Casteglione <jrmsdev@gmail.com> */
/* See LICENSE file. */

#include "lnfs_passthrough.hpp"

#include <cerrno>

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

int lnfs_getattr(const char* path, struct stat* stbuf, fuse_file_info* fi)
{
	lnfs_debug("passthrough getattr: {}", path);
	(void) fi;
	int res;
	res = lstat(path, stbuf);
	if (res == -1)
		return -errno;
	return 0;
}

//------------------------------------------------------------------------------

static const fuse_operations ops = {
	.getattr = lnfs_getattr,
	.init    = lnfs_init,
};

const fuse_operations* lnfs_operations()
{
	lnfs_debug("passthrough operations");
	return &ops;
}
