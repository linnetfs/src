/* Copyright (c) Jeremías Casteglione <jrmsdev@gmail.com> */
/* See LICENSE file. */

// https://libfuse.github.io/doxygen/passthrough_8c_source.html

#pragma once

#include "lnfs_fuse.hpp"

void* lnfs_init(struct fuse_conn_info* conn, struct fuse_config* cfg);

int lnfs_getattr(const char* path, struct stat* stbuf, struct fuse_file_info* fi);

int lnfs_access(const char* path, int mask);

int lnfs_readlink(const char* path, char* buf, size_t size);

int lnfs_readdir(const char* path, void* buf, fuse_fill_dir_t filler,
		off_t offset, struct fuse_file_info* fi, enum fuse_readdir_flags flags);

int lnfs_mknod(const char* path, mode_t mode, dev_t rdev);

int lnfs_mkdir(const char* path, mode_t mode);

int lnfs_unlink(const char* path);

int lnfs_rmdir(const char* path);

int lnfs_symlink(const char* target, const char* path);

int lnfs_rename(const char* oldpath, const char* newpath, unsigned int flags);

int lnfs_link(const char* target, const char* path);

int lnfs_chmod(const char* path, mode_t mode, struct fuse_file_info* fi);

int lnfs_chown(const char* path, uid_t uid, gid_t gid, struct fuse_file_info* fi);

const fuse_operations* lnfs_operations();
