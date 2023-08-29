/* Copyright (c) Jeremías Casteglione <jrmsdev@gmail.com> */
/* See LICENSE file. */

#include "lnfs_memory.hpp"
#include "lnfs_log.hpp"

//------------------------------------------------------------------------------
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

//------------------------------------------------------------------------------
// operations

static const struct fuse_operations ops = {
	.init = lnfs_init,
};

const struct fuse_operations* lnfs_operations()
{
	lnfs_debug("memory operations");
	return &ops;
}
