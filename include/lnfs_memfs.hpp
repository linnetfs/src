/* Copyright (c) Jeremías Casteglione <jrmsdev@gmail.com> */
/* See LICENSE file. */

#pragma once

#include "lnfs_dir.hpp"

using namespace std;

class LNFSMem {
	public:
		File lookup(string p);
		LNFSMem(string n);

	private:
		string name;
		Dir root;
};
