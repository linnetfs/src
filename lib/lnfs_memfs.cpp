/* Copyright (c) Jeremías Casteglione <jrmsdev@gmail.com> */
/* See LICENSE file. */

#include "lnfs_memfs.hpp"
#include "lnfs_log.hpp"

/******************************************************************************/
// lookup

File LNFSMem::lookup(string p)
{
	lnfs_debug("lnfs lookup {}, count: {}", p, count);
	for (int idx = 0; idx < count; idx++)
	{
		File f = files[idx];
		lnfs_debug("lnfs lookup check path {}", f.name());
		if (f.is_path(p))
			return f;
	}
	lnfs_debug("lnfs lookup {}: no such file or directory", p);
	File f(p);
	f.not_found();
	return f;
}

/******************************************************************************/
// LNFSMem

LNFSMem::LNFSMem(string n)
{
	name = n;
	File root("/");
	root.dirent();
	count = 0;
	files[count++] = root;
}
