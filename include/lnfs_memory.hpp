/* Copyright (c) Jeremías Casteglione <jrmsdev@gmail.com> */
/* See LICENSE file. */

// https://github.com/libfuse/libfuse/blob/master/example/null.c
// https://github.com/libfuse/libfuse/blob/master/example/hello.c

#pragma once

#include "lnfs_fuse.hpp"

const struct fuse_operations* lnfs_operations();

void* lnfs_init(struct fuse_conn_info* conn, struct fuse_config* cfg);

int lnfs_getattr(const char* path, struct stat* stbuf, struct fuse_file_info* fi);

int lnfs_access(const char* path, int mask);

int lnfs_readdir(const char* path, void* buf, fuse_fill_dir_t filler,
		off_t offset, struct fuse_file_info* fi, enum fuse_readdir_flags flags);

int lnfs_readlink(const char* path, char* buf, size_t size);
