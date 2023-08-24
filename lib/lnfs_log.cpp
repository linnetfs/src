/* Copyright (c) Jeremías Casteglione <jrmsdev@gmail.com> */
/* See LICENSE file. */

#include <iostream>

#include "lnfs_log.h"

void lnfs_debug(string msg)
{
#ifdef LNFS_DEBUG
	cerr << "[D] " << msg << endl;
#endif
}

void lnfs_log(string msg)
{
	cout << msg << endl;
}
