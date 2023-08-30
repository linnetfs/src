/* Copyright (c) Jeremías Casteglione <jrmsdev@gmail.com> */
/* See LICENSE file. */

// https://github.com/libfuse/libfuse/blob/master/example/null.c
// https://github.com/libfuse/libfuse/blob/master/example/hello.c

#pragma once

#include "lnfs_fuse.hpp"

#include <time.h>

#include <string>

using namespace std;

/******************************************************************************/
// File

class File {
	public:
		int mode;
		int nlink;
		int uid;
		int gid;
		int size;
		int blocks;
		time_t atime;
		time_t mtime;
		time_t ctime;
		bool not_found;

		int getattr(struct stat* stbuf);

		void dirent();
		bool is_dir();
		bool is_path(string path);
		string name();

		File();
		File(string p);

	private:
		string path;
		bool dir;
		void init(string p);
};

File lnfs_dir(string path);

/******************************************************************************/
// LNFS

class LNFS {
	public:
		File get(string p);
		LNFS(string n);

	private:
		string name;
		int next;
		File files[1000];
};

/******************************************************************************/
// fuse

const struct fuse_operations* lnfs_operations();

void* lnfs_init(struct fuse_conn_info* conn, struct fuse_config* cfg);

int lnfs_getattr(const char* path, struct stat* stbuf, struct fuse_file_info* fi);
