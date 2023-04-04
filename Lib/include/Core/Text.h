#pragma once
#ifndef UNICODE
#define UNICODE
#endif

#include <string>
#include <fmt/core.h>
#include <fmt/xchar.h>
#include <boost/utility/string_view.hpp>

#define CTEXT(str) L##str
#define CFORMAT( str, ... ) fmt::format( CTEXT(str), __VA_ARGS__ )
#define cchar_t wchar_t
#define ctext_t const cchar_t*
namespace core
{
	using string_t = std::basic_string<cchar_t, std::char_traits<cchar_t>, std::allocator<cchar_t>>;
	using string_view_t = boost::basic_string_view<cchar_t, std::char_traits<cchar_t>>;
	using id_t = string_t;
}


