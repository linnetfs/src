/* Copyright (c) Jeremías Casteglione <jrmsdev@gmail.com> */
/* See LICENSE file. */

#include "lnfs_file.hpp"
#include "lnfs_log.hpp"

/******************************************************************************/
// getattr

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

/******************************************************************************/
// access

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

/******************************************************************************/
// dirent

void File::dirent()
{
	dir = true;
	mode = 0755;
}

/******************************************************************************/
// is_dir

bool File::is_dir()
{
	return dir;
}

/******************************************************************************/
// is_path

bool File::is_path(string p)
{
	lnfs_debug("{} is_path: {}", path, p);
	return path == p;
}

/******************************************************************************/
// not_found

void File::not_found()
{
	found = false;
}

/******************************************************************************/
// is_not_found

bool File::is_not_found()
{
	return !found;
}

/******************************************************************************/
// name

string File::name()
{
	return path;
}

/******************************************************************************/
// init

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

/******************************************************************************/
// File

File::File()
{
	init("/");
}

File::File(string p)
{
	init(p);
}
