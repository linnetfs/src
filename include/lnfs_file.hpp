/* Copyright (c) Jeremías Casteglione <jrmsdev@gmail.com> */
/* See LICENSE file. */

#pragma once

#include <string>
#include <sys/stat.h>
#include <time.h>

using namespace std;

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

		int getattr(struct stat* stbuf);
		int access(int mask);

		void dirent();
		bool is_dir();
		bool is_path(string path);
		void not_found();
		bool is_not_found();
		string name();

		File();
		File(string p);

	private:
		string path;
		bool dir;
		void init(string p);
		bool found;
};
