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
		lnfs_error("passthrough getattr {} {}", path, -errno);
		return -errno;
	}
	lnfs_debug("passthrough getattr {} mode={} nlink={} uid={} gid={} size={} blocks={} atime={} mtime={} ctime={}",
		path, stbuf->st_mode, stbuf->st_nlink, stbuf->st_uid, stbuf->st_gid,
		stbuf->st_size, stbuf->st_blocks, stbuf->st_atime, stbuf->st_mtime,
		stbuf->st_ctime);
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
		lnfs_error("passthrough access {} {} {}", path, mask, -errno);
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
		lnfs_error("passthrough readlink {} {}", path, -errno);
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
		lnfs_error("passthrough readdir {} opendir {}", path, -errno);
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
		lnfs_error("passthrough mknod {} {} {}", path, mode, -errno);
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
		lnfs_error("passthrough mkdir {} {} {}", path, mode, -errno);
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
		lnfs_error("passthrough unlink {} {}", path, -errno);
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
		lnfs_error("passthrough rmdir {} {}", path, -errno);
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
		lnfs_error("passthrough symlink {} -> {} {}", path, target, -errno);
		return -errno;
	}
	return 0;
}

//------------------------------------------------------------------------------
// man 2 rename

int lnfs_rename(const char* oldpath, const char* newpath, unsigned int flags)
{
	lnfs_debug("passthrough rename {} -> {}", oldpath, newpath);
	if (flags)
	{
		lnfs_error("passthrough rename {} -> {} invalid flags {}", oldpath, newpath, EINVAL);
		return -EINVAL;
	}
	int res;
	res = rename(oldpath, newpath);
	if (res == -1)
	{
		lnfs_error("passthrough rename {} -> {} {}", oldpath, newpath, -errno);
		return -errno;
	}
	return 0;
}

//------------------------------------------------------------------------------
// man 2 link

int lnfs_link(const char* target, const char* path)
{
	lnfs_debug("passthrough link {} -> {}", path, target);
	int res;
	res = link(target, path);
	if (res == -1)
	{
		lnfs_error("passthrough link {} -> {} {}", path, target, -errno);
		return -errno;
	}
	return 0;
}

//------------------------------------------------------------------------------
// man 2 chmod

int lnfs_chmod(const char* path, mode_t mode, struct fuse_file_info* fi)
{
	lnfs_debug("passthrough chmod {} {}", path, mode);
	(void) fi;
	int res;
	res = chmod(path, mode);
	if (res == -1)
	{
		lnfs_error("passthrough chmod {} {} {}", path, mode, -errno);
		return -errno;
	}
	return 0;
}

//------------------------------------------------------------------------------
// man 2 lchown

int lnfs_chown(const char* path, uid_t uid, gid_t gid, struct fuse_file_info* fi)
{
	lnfs_debug("passthrough chown {} {} {}", path, uid, gid);
	(void) fi;
	int res;
	res = lchown(path, uid, gid);
	if (res == -1)
	{
		lnfs_error("passthrough chown {} {} {} {}", path, uid, gid, -errno);
		return -errno;
	}
	return 0;
}

//------------------------------------------------------------------------------
// man 2 truncate

int lnfs_truncate(const char* path, off_t size, struct fuse_file_info* fi)
{
	lnfs_debug("passthrough truncate {} {}", path, size);
	(void) fi;
	int res;
	if (fi != NULL)
		res = ftruncate(fi->fh, size);
	else
		res = truncate(path, size);
	if (res == -1)
	{
		lnfs_error("passthrough truncate {} {} {}", path, size, -errno);
		return -errno;
	}
	return 0;
}

//------------------------------------------------------------------------------
// man 2 utimensat

#ifdef HAVE_UTIMENSAT
int lnfs_utimens(const char* path, const struct timespec ts[2],
		struct fuse_file_info* fi)
{
	lnfs_debug("passthrough utimens {}", path);
	(void) fi;
	int res;
	res = utimensat(0, path, ts, AT_SYMLINK_NOFOLLOW);
	if (res == -1)
	{
		lnfs_error("passthrough utimens {} {}", path, -errno);
		return -errno;
	}
	return 0;
}
#endif

//------------------------------------------------------------------------------
// man 2 creat

int lnfs_create(const char* path, mode_t mode, struct fuse_file_info* fi)
{
	lnfs_debug("passthrough create {} {}", path, mode);
	int res;
	res = open(path, fi->flags, mode);
	if (res == -1)
	{
		lnfs_error("passthrough create {} {} {}", path, mode, -errno);
		return -errno;
	}
	fi->fh = res;
	return 0;
}

//------------------------------------------------------------------------------
// man 2 open

int lnfs_open(const char* path, struct fuse_file_info* fi)
{
	lnfs_debug("passthrough open {}", path);
	int res;
	res = open(path, fi->flags);
	if (res == -1)
	{
		lnfs_error("passthrough open {} {}", path, -errno);
		return -errno;
	}
	fi->fh = res;
	return 0;
}

//------------------------------------------------------------------------------
// man 2 pread

int lnfs_read(const char* path, char* buf, size_t size, off_t offset,
		struct fuse_file_info* fi)
{
	lnfs_debug("passthrough read {}", path);
	int fd;
	int res;
	if (fi == NULL)
		fd = open(path, O_RDONLY);
	else
		fd = fi->fh;
	if (fd == -1)
	{
		lnfs_error("passthrough read {} open {}", path, -errno);
		return -errno;
	}
	res = pread(fd, buf, size, offset);
	if (res == -1)
	{
		lnfs_error("passthrough read {} {}", path, -errno);
		res = -errno;
	}
	if (fi == NULL)
		close(fd);
	return res;
}

//------------------------------------------------------------------------------
// man 2 pwrite

int lnfs_write(const char* path, const char* buf, size_t size, off_t offset,
		struct fuse_file_info* fi)
{
	lnfs_debug("passthrough write {}", path);
	int fd;
	int res;
	if (fi == NULL)
		fd = open(path, O_RDONLY);
	else
		fd = fi->fh;
	if (fd == -1)
	{
		lnfs_error("passthrough write {} open {}", path, -errno);
		return -errno;
	}
	res = pwrite(fd, buf, size, offset);
	if (res == -1)
	{
		lnfs_error("passthrough write {} {}", path, -errno);
		res = -errno;
	}
	if (fi == NULL)
		close(fd);
	return res;
}

//------------------------------------------------------------------------------
// man 2 statvfs

int lnfs_statfs(const char* path, struct statvfs* stbuf)
{
	lnfs_debug("passthrough statfs {}", path);
	int res;
	res = statvfs(path, stbuf);
	if (res == -1)
	{
		lnfs_error("passthrough statfs {} {}", path, -errno);
		return -errno;
	}
	return 0;
}

//------------------------------------------------------------------------------
// man 2 close

int lnfs_release(const char* path, struct fuse_file_info* fi)
{
	lnfs_debug("passthrough release {}", path);
	(void) path;
	close(fi->fh);
	return 0;
}

//------------------------------------------------------------------------------
// man 2 lseek

off_t lnfs_lseek(const char* path, off_t off, int whence,
		struct fuse_file_info* fi)
{
	lnfs_debug("passthrough lseek {}", path);
	int fd;
	off_t res;
	if (fi == NULL)
		fd = open(path, O_RDONLY);
	else
		fd = fi->fh;
	if (fd == -1)
	{
		lnfs_error("passthrough lseek {} open {}", path, -errno);
		return -errno;
	}
	res = lseek(fd, off, whence);
	if (res == -1)
	{
		lnfs_error("passthrough lseek {} {}", path, -errno);
		res = -errno;
	}
	if (fi == NULL)
		close(fd);
	return res;
}

//------------------------------------------------------------------------------

static const struct fuse_operations ops = {
	.getattr  = lnfs_getattr,
	.readlink = lnfs_readlink,
	.mknod    = lnfs_mknod,
	.mkdir    = lnfs_mkdir,
	.unlink   = lnfs_unlink,
	.rmdir    = lnfs_rmdir,
	.symlink  = lnfs_symlink,
	.rename   = lnfs_rename,
	.link     = lnfs_link,
	.chmod    = lnfs_chmod,
	.chown    = lnfs_chown,
	.truncate = lnfs_truncate,
	.open     = lnfs_open,
	.read     = lnfs_read,
	.write    = lnfs_write,
	.statfs   = lnfs_statfs,
	.release  = lnfs_release,
	.readdir  = lnfs_readdir,
	.init     = lnfs_init,
	.access   = lnfs_access,
	.create   = lnfs_create,
#ifdef HAVE_UTIMENSAT
	.utimens  = lnfs_utimens,
#endif
	.lseek    = lnfs_lseek,
};

const struct fuse_operations* lnfs_operations()
{
	lnfs_debug("passthrough operations");
	return &ops;
}
