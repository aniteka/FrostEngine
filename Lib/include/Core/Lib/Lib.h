#pragma once
#include <boost/format.hpp>

namespace Core
{
	namespace bst = boost;

	using namespace std::string_literals;
	inline bst::format operator""_f(const char* ch, size_t len)
	{
		return bst::format(ch);
	}

	template<auto Fn>
	struct DeleteWith
	{
		template<class T>
		constexpr void operator()(T* ptr)
		{
			Fn(ptr);
		}
	};
}
