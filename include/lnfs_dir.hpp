/* Copyright (c) Jeremías Casteglione <jrmsdev@gmail.com> */
/* See LICENSE file. */

#pragma once

#include "lnfs_file.hpp"

class Dir {
	public:
		File lookup(string p);
		Dir();
		Dir(string path);

	private:
		int count;
		File fh;
		File files[1000];
};
