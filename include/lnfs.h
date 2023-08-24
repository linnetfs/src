/* Copyright (c) Jeremías Casteglione <jrmsdev@gmail.com> */
/* See LICENSE file. */

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

#include <string>

using namespace std;

string lnfs_name();

#endif // LNFS_H_
