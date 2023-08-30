/* Copyright (c) Jeremías Casteglione <jrmsdev@gmail.com> */
/* See LICENSE file. */

#include "lnfs_memory.hpp"

#include "lnfs_file.hpp"
#include "lnfs_log.hpp"

/******************************************************************************/
// LNFS

static LNFS fs("memory");

/******************************************************************************/
// init

void* lnfs_init(struct fuse_conn_info* conn, struct fuse_config* cfg)
{
	lnfs_debug("memory init");
	(void) conn;
	cfg->use_ino = 1;
	cfg->entry_timeout = 0;
	cfg->attr_timeout = 0;
	cfg->negative_timeout = 0;
	return nullptr;
}

/******************************************************************************/
// getattr

int lnfs_getattr(const char* path, struct stat* stbuf, struct fuse_file_info* fi)
{
	lnfs_debug("memory getattr {}", path);
	File f = fs.lookup(path);
	if (f.is_not_found())
	{
		int rc = -ENOENT;
		lnfs_error("memory getattr {} {}", f.name(), rc);
		return rc;
	}
	return f.getattr(stbuf);
}

/******************************************************************************/
// access

int lnfs_access(const char* path, int mask)
{
	lnfs_debug("memory access {} {}", path, mask);
	File f = fs.lookup(path);
	if (f.is_not_found())
	{
		int rc = -ENOENT;
		lnfs_error("memory access {} {}", f.name(), rc);
		return rc;
	}
	return f.access(mask);
}

/******************************************************************************/
// operations

static const struct fuse_operations ops = {
	.getattr = lnfs_getattr,
	.init    = lnfs_init,
	.access  = lnfs_access,
};

const struct fuse_operations* lnfs_operations()
{
	lnfs_debug("memory operations");
	return &ops;
}
