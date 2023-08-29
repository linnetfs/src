/* Copyright (c) Jeremías Casteglione <jrmsdev@gmail.com> */
/* See LICENSE file. */

// https://github.com/libfuse/libfuse/blob/master/example/null.c
// https://github.com/libfuse/libfuse/blob/master/example/hello.c

#pragma once

#include "lnfs_fuse.hpp"

const struct fuse_operations* lnfs_operations();

void* lnfs_init(struct fuse_conn_info* conn, struct fuse_config* cfg);
