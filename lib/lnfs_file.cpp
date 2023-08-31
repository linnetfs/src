/* Copyright (c) Jeremías Casteglione <jrmsdev@gmail.com> */
/* See LICENSE file. */

#include "lnfs_file.hpp"
#include "lnfs_log.hpp"

#include <unistd.h> // for: R_OK and friends

/******************************************************************************/
// File::getattr
//   man 7 inode

int File::getattr(struct stat* stbuf)
{
	lnfs_debug("file getattr {}", name());
	stbuf->st_mode = mode;
	stbuf->st_nlink = nlink;
	stbuf->st_uid = uid;
	stbuf->st_gid = gid;
	stbuf->st_size = size;
	stbuf->st_blocks = blocks;
	stbuf->st_atime = atime;
	stbuf->st_mtime = mtime;
	stbuf->st_ctime = ctime;
	lnfs_debug("file getattr {} mode={} nlink={} uid={} gid={} size={} blocks={} atime={} mtime={} ctime={}",
		name(), stbuf->st_mode, stbuf->st_nlink, stbuf->st_uid, stbuf->st_gid,
		stbuf->st_size, stbuf->st_blocks, stbuf->st_atime, stbuf->st_mtime,
		stbuf->st_ctime);
	return 0;
}

/******************************************************************************/
// File::access

int File::access(int mask)
{
	// Stolen from gnu/glibc/io/access.c
	int perms = R_OK | W_OK | X_OK | F_OK;
	lnfs_debug("file access {} {} {}", name(), perms, mask);
	if ((mask & ~perms) != 0)
	{
		int rc = -EACCES;
		lnfs_error("file access {} {} {} {}", name(), perms, mask, rc);
		return rc;
	}
	return 0;
}

/******************************************************************************/
// File::dirent

void File::dirent()
{
	mode = S_IFDIR | 0755;
}

/******************************************************************************/
// File::is_dir

bool File::is_dir()
{
	// man 7 indoe (The file type and mode)
	if (S_ISDIR(mode))
		return true;
	return false;
}

/******************************************************************************/
// File::is_path

bool File::is_path(string p)
{
	lnfs_debug("{} is_path: {}", path, p);
	return path == p;
}

/******************************************************************************/
// File::not_found

void File::not_found()
{
	found = false;
}

/******************************************************************************/
// File::is_not_found

bool File::is_not_found()
{
	return !found;
}

/******************************************************************************/
// File::name

string File::name()
{
	return path;
}

/******************************************************************************/
// File::init

void File::init(string p)
{
	path = p;
	mode = S_IFREG | 0644;
	nlink = 1;
	uid = 0;
	gid = 0;
	size = 0;
	blocks = 0;
	atime = mtime = ctime = time(NULL);
	found = true;
}

/******************************************************************************/
// File::File

File::File()
{
	init("/");
	dirent();
}

File::File(string p)
{
	init(p);
}
