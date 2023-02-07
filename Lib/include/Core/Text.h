#pragma once
#ifndef UNICODE
#define UNICODE
#endif

#include <string>
#include <sstream>

#define TEXT(str) L##str
#define CHAR_TYPE wchar_t
#define TEXT_TYPE CHAR_TYPE*
namespace Core
{
	using StringType = std::basic_string<CHAR_TYPE, std::char_traits<CHAR_TYPE>, std::allocator<CHAR_TYPE>>;
	using StringStreamType = std::basic_stringstream<CHAR_TYPE, std::char_traits<CHAR_TYPE>, std::allocator<CHAR_TYPE>>;
	using StringIStreamType = std::basic_istringstream<CHAR_TYPE, std::char_traits<CHAR_TYPE>, std::allocator<CHAR_TYPE>>;
	using StringOStreamType = std::basic_ostringstream<CHAR_TYPE, std::char_traits<CHAR_TYPE>, std::allocator<CHAR_TYPE>>;
}


