/* Copyright (c) Jeremías Casteglione <jrmsdev@gmail.com> */
/* See LICENSE file. */

#pragma once

#include "lnfs_file.hpp"

using namespace std;

class LNFS {
	public:
		File lookup(string p);
		LNFS(string n);

	private:
		string name;
		int count;
		int next;
		File files[1000];
};
