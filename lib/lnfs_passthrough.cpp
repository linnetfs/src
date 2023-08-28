/* Copyright (c) Jeremías Casteglione <jrmsdev@gmail.com> */
/* See LICENSE file. */

#include "lnfs_passthrough.hpp"

#include <cerrno>
#include <dirent.h> // for: readdir
#include <string.h> // for: memset
#include <unistd.h> // for: access

#include "lnfs_log.hpp"

//------------------------------------------------------------------------------

void* lnfs_init(struct fuse_conn_info* conn, struct fuse_config* cfg)
{
	lnfs_debug("passthrough init");
	(void) conn;
	cfg->use_ino = 1;
	cfg->entry_timeout = 0;
	cfg->attr_timeout = 0;
	cfg->negative_timeout = 0;
	return nullptr;
}

//------------------------------------------------------------------------------
// man 2 lstat

int lnfs_getattr(const char* path, struct stat* stbuf, struct fuse_file_info* fi)
{
	lnfs_debug("passthrough getattr {}", path);
	(void) fi;
	int res;
	res = lstat(path, stbuf);
	if (res == -1)
	{
		lnfs_error("passthrough getattr {} {}", path, errno);
		return -errno;
	}
	return 0;
}

//------------------------------------------------------------------------------
// man 2 access

int lnfs_access(const char* path, int mask)
{
	lnfs_debug("passthrough access {} {}", path, mask);
	int res;
	res = access(path, mask);
	if (res == -1)
	{
		lnfs_error("passthrough access {} {} {}", path, mask, errno);
		return -errno;
	}
	return 0;
}

//------------------------------------------------------------------------------
// man 2 readlink

int lnfs_readlink(const char* path, char* buf, size_t size)
{
	lnfs_debug("passthrough readlink {}", path);
	int res;
	res = readlink(path, buf, size - 1);
	if (res == -1)
	{
		lnfs_error("passthrough readlink {} {}", path, errno);
		return -errno;
	}
	buf[res] = '\0';
	return 0;
}

//------------------------------------------------------------------------------
// man 3 readdir

int lnfs_readdir(const char* path, void* buf, fuse_fill_dir_t filler,
		off_t offset, struct fuse_file_info* fi, enum fuse_readdir_flags flags)
{
	lnfs_debug("passthrough readdir {}", path);

	DIR* dp;
	struct dirent* de;

	(void) offset;
	(void) fi;
	(void) flags;

	dp = opendir(path);
	if (dp == NULL)
	{
		lnfs_error("passthrough readdir {} opendir {}", path, errno);
		return -errno;
	}

	while ((de = readdir(dp)) != NULL)
	{
		struct stat st;
		memset(&st, 0, sizeof(st));
		st.st_ino = de->d_ino;
		st.st_mode = de->d_type << 12;
		if (filler(buf, de->d_name, &st, 0, FUSE_FILL_DIR_PLUS))
			break;
	}

	closedir(dp);
	return 0;
}

//------------------------------------------------------------------------------
// man 2 mknod

int lnfs_mknod(const char* path, mode_t mode, dev_t rdev)
{
	lnfs_debug("passthrough mknod {} {}", path, mode);
	int res;
	res = mknod(path, mode, rdev);
	if (res == -1)
	{
		lnfs_error("passthrough mknod {} {} {}", path, mode, errno);
		return -errno;
	}
	return 0;
}

//------------------------------------------------------------------------------
// man 2 mkdir

int lnfs_mkdir(const char* path, mode_t mode)
{
	lnfs_debug("passthrough mkdir {} {}", path, mode);
	int res;
	res = mkdir(path, mode);
	if (res == -1)
	{
		lnfs_error("passthrough mkdir {} {} {}", path, mode, errno);
		return -errno;
	}
	return 0;
}

//------------------------------------------------------------------------------
// man 2 unlink

int lnfs_unlink(const char* path)
{
	lnfs_debug("passthrough unlink {}", path);
	int res;
	res = unlink(path);
	if (res == -1)
	{
		lnfs_error("passthrough unlink {} {}", path, errno);
		return -errno;
	}
	return 0;
}

//------------------------------------------------------------------------------
// man 2 rmdir

int lnfs_rmdir(const char* path)
{
	lnfs_debug("passthrough rmdir {}", path);
	int res;
	res = rmdir(path);
	if (res == -1)
	{
		lnfs_error("passthrough rmdir {} {}", path, errno);
		return -errno;
	}
	return 0;
}

//------------------------------------------------------------------------------
// man 2 symlink

int lnfs_symlink(const char* target, const char* path)
{
	lnfs_debug("passthrough symlink {} -> {}", path, target);
	int res;
	res = symlink(target, path);
	if (res == -1)
	{
		lnfs_error("passthrough symlink {} -> {} {}", path, target, errno);
		return -errno;
	}
	return 0;
}

//------------------------------------------------------------------------------

static const fuse_operations ops = {
	.getattr  = lnfs_getattr,
	.readlink = lnfs_readlink,
	.mknod    = lnfs_mknod,
	.mkdir    = lnfs_mkdir,
	.unlink   = lnfs_unlink,
	.rmdir    = lnfs_rmdir,
	.symlink  = lnfs_symlink,
	.readdir  = lnfs_readdir,
	.init     = lnfs_init,
	.access   = lnfs_access,
};

const fuse_operations* lnfs_operations()
{
	lnfs_debug("passthrough operations");
	return &ops;
}
