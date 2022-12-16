// Lib.h : Header file for your target.

#pragma once
#include <iostream>
#include <memory>
#include <functional>
#include <string>
#include <boost/format.hpp>

namespace Core
{
	namespace bst = boost;

	using uint8		= unsigned char;
	using int8		= signed char;
	using uint16	= unsigned short;
	using int16		= signed short;
	using uint32	= unsigned long;
	using int32		= signed long;
	using uint64	= unsigned long long;
	using int64		= long long;
	using uint		= uint32;

	using namespace std::string_literals;
	inline const bst::format operator"" f(const char* ch, size_t len)
	{
		return bst::format(ch);
	}

	template<auto Fn>
	struct DeleteWith
	{
		template<class T>
		constexpr void operator()(T* int_ptr)
		{
			Fn(int_ptr);
		}
	};

}
