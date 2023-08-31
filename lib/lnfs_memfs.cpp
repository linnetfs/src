/* Copyright (c) Jeremías Casteglione <jrmsdev@gmail.com> */
/* See LICENSE file. */

#include "lnfs_memfs.hpp"
#include "lnfs_log.hpp"

/******************************************************************************/
// LNFSMem::lookup
//   man 7 path_resolution

File LNFSMem::lookup(string p)
{
	lnfs_debug("lnfs lookup {}", p);
	File f(p);
	f.not_found();
	return f;
}

/******************************************************************************/
// LNFSMem::LNFSMem

LNFSMem::LNFSMem(string n)
{
	name = n;
	root = Dir("/");
}
