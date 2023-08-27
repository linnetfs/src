/* Copyright (c) Jeremías Casteglione <jrmsdev@gmail.com> */
/* See LICENSE file. */

#include "lnfs_passthrough.hpp"

void* lnfs_init(fuse_conn_info* conn, fuse_config* cfg)
{
	return nullptr;
}

fuse_operations ops = {
	.init = lnfs_init,
};

fuse_operations* lnfs_operations()
{
	return &ops;
}
