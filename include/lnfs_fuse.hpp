/* Copyright (c) Jeremías Casteglione <jrmsdev@gmail.com> */
/* See LICENSE file. */

#pragma once

#define FUSE_USE_VERSION 35

#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

#ifdef linux
/* For pread()/pwrite()/utimensat() */
#define _XOPEN_SOURCE 700
#endif

#include <fuse.h>

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

int lnfs_truncate(const char* path, off_t size, struct fuse_file_info* fi);

int lnfs_open(const char* path, struct fuse_file_info* fi);

int lnfs_read(const char* path, char* buf, size_t size, off_t offset,
		struct fuse_file_info* fi);

int lnfs_create(const char* path, mode_t mode, struct fuse_file_info* fi);

#ifdef HAVE_UTIMENSAT
int lnfs_utimens(const char* path, const struct timespec ts[2],
		struct fuse_file_info* fi);
#endif

const struct fuse_operations* lnfs_operations();
