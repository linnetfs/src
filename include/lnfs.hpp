/* Copyright (c) Jeremías Casteglione <jrmsdev@gmail.com> */
/* See LICENSE file. */

#pragma once

#include "lnfs_build.hpp"
#include "lnfs_fuse.hpp"

#ifdef LNFS_MODE_passthrough
#include "lnfs_passthrough.hpp"
#endif

#ifdef LNFS_MODE_memory
#include "lnfs_memory.hpp"
#endif
