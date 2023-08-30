/* Copyright (c) Jeremías Casteglione <jrmsdev@gmail.com> */
/* See LICENSE file. */

#include "lnfs_file.hpp"
#include "lnfs_log.hpp"

/******************************************************************************/
// File

int File::getattr(struct stat* stbuf)
{
	stbuf->st_mode = mode;
	stbuf->st_nlink = nlink;
	stbuf->st_uid = uid;
	stbuf->st_gid = gid;
	stbuf->st_size = size;
	stbuf->st_blocks = blocks;
	stbuf->st_atime = atime;
	stbuf->st_mtime = mtime;
	stbuf->st_ctime = ctime;
	return 0;
}

int File::access(int mask)
{
	lnfs_debug("file access {} {} {}", name(), ACCESSPERMS, mask);
	if ((mask & ~ACCESSPERMS) != 0)
	{
		int rc = -EACCES;
		lnfs_error("file access {} {} {} {}", name(), ACCESSPERMS, mask, rc);
		return rc;
	}
	return 0;
}

void File::dirent()
{
	dir = true;
	mode = 0755;
}

bool File::is_dir()
{
	return dir;
}

bool File::is_path(string p)
{
	lnfs_debug("{} is_path: {}", path, p);
	return path == p;
}

void File::not_found()
{
	found = false;
}

bool File::is_not_found()
{
	return !found;
}

string File::name()
{
	return path;
}

void File::init(string p)
{
	path = p;
	dir  = false;
	mode = 0644;
	nlink = 1;
	uid = 0;
	gid = 0;
	size = 0;
	blocks = 0;
	atime = mtime = ctime = time(NULL);
	found = true;
}

File::File()
{
	init("/");
}

File::File(string p)
{
	init(p);
}

/******************************************************************************/
// LNFS

File LNFS::lookup(string p)
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

LNFS::LNFS(string n)
{
	name = n;
	File root("/");
	root.dirent();
	files[0] = root;
	count = 1;
	next = 1;
}
