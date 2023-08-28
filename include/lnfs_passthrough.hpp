/* Copyright (c) Jeremías Casteglione <jrmsdev@gmail.com> */
/* See LICENSE file. */

// https://libfuse.github.io/doxygen/passthrough_8c_source.html

#pragma once

#include "lnfs_fuse.hpp"

void* lnfs_init(fuse_conn_info* conn, fuse_config* cfg);

int lnfs_getattr(const char* path, struct stat* stbuf, fuse_file_info* fi);

int lnfs_access(const char* path, int mask);

int lnfs_readlink(const char* path, char* buf, size_t size);

const fuse_operations* lnfs_operations();
