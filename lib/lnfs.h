#ifndef LNFS_H_
#define LNFS_H_

#define FUSE_USE_VERSION 31

#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

#ifdef linux
/* For pread()/pwrite()/utimensat() */
#define _XOPEN_SOURCE 700
#endif

#include <fuse.h>

#endif // LNFS_H_
