/* Copyright (c) Jeremías Casteglione <jrmsdev@gmail.com> */
/* See LICENSE file. */

#include "lnfs_log.h"

#include <iostream>

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
