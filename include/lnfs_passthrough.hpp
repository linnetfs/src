/* Copyright (c) Jeremías Casteglione <jrmsdev@gmail.com> */
/* See LICENSE file. */

// https://libfuse.github.io/doxygen/passthrough_8c_source.html

#pragma once

#include "lnfs_fuse.hpp"

void* lnfs_init(fuse_conn_info*, fuse_config*);

int lnfs_getattr(const char* path, struct stat* stbuf, fuse_file_info* fi);

const fuse_operations* lnfs_operations();
