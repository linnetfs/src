/* Copyright (c) Jeremías Casteglione <jrmsdev@gmail.com> */
/* See LICENSE file. */

#include "lnfs_memory.hpp"
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

static LNFS fs("memory");

/******************************************************************************/
// init

void* lnfs_init(struct fuse_conn_info* conn, struct fuse_config* cfg)
{
	lnfs_debug("memory init");
	(void) conn;
	cfg->use_ino = 1;
	cfg->entry_timeout = 0;
	cfg->attr_timeout = 0;
	cfg->negative_timeout = 0;
	return nullptr;
}

/******************************************************************************/
// getattr

int lnfs_getattr(const char* path, struct stat* stbuf, struct fuse_file_info* fi)
{
	lnfs_debug("memory getattr {}", path);
	File f = fs.lookup(path);
	if (f.is_not_found())
	{
		int rc = -ENOENT;
		lnfs_error("memory getattr {} {}", f.name(), rc);
		return rc;
	}
	return f.getattr(stbuf);
}

/******************************************************************************/
// operations

static const struct fuse_operations ops = {
	.getattr = lnfs_getattr,
	.init    = lnfs_init,
};

const struct fuse_operations* lnfs_operations()
{
	lnfs_debug("memory operations");
	return &ops;
}
