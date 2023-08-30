/* Copyright (c) Jeremías Casteglione <jrmsdev@gmail.com> */
/* See LICENSE file. */

// https://github.com/libfuse/libfuse/blob/master/example/null.c
// https://github.com/libfuse/libfuse/blob/master/example/hello.c

#pragma once

#include "lnfs_fuse.hpp"

#include <string>

using namespace std;

/******************************************************************************/
// File

class File {
	public:
		void dirent();
		bool is_dir();
		File(string p);

	private:
		string path;
		bool dir;
};

File lnfs_dir(string path);

/******************************************************************************/
// LNFS

class LNFS {
	public:
		LNFS();

	private:
		int next;
		File* files[1000];
};

/******************************************************************************/
// fuse

const struct fuse_operations* lnfs_operations();

void* lnfs_init(struct fuse_conn_info* conn, struct fuse_config* cfg);
