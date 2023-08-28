/* Copyright (c) Jeremías Casteglione <jrmsdev@gmail.com> */
/* See LICENSE file. */

#pragma once

#include <format>
#include <iostream>

template<typename... Args>
void lnfs_debug(const std::format_string<Args...> fmt, Args&&... args)
{
#ifdef LNFS_DEBUG
	std::cerr << "[D] " << std::vformat(fmt.get(), std::make_format_args(args...)) << std::endl;
#endif
}

template<typename... Args>
void lnfs_error(const std::format_string<Args...> fmt, Args&&... args)
{
	std::cerr << "[E] " << std::vformat(fmt.get(), std::make_format_args(args...)) << std::endl;
}
