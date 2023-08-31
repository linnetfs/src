/* Copyright (c) Jeremías Casteglione <jrmsdev@gmail.com> */
/* See LICENSE file. */

#include "lnfs_dir.hpp"
#include "lnfs_log.hpp"

/******************************************************************************/
// Dir::lookup
//   man 7 path_resolution

File Dir::lookup(string p)
{
	lnfs_debug("lnfs_dir lookup {}, count: {}", p, count);
	for (int idx = 0; idx < count; idx++)
	{
		File f = files[idx];
		lnfs_debug("lnfs_dir lookup check path {}", f.name());
		if (f.is_path(p))
			return f;
	}
	lnfs_debug("lnfs_dir lookup {}: no such file or directory", p);
	File f(p);
	f.not_found();
	return f;
}

/******************************************************************************/
// Dir::Dir

Dir::Dir()
{
	fh = File("/");
	fh.dirent();
}

Dir::Dir(string path)
{
	fh = File(path);
	fh.dirent();
}
