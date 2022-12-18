#pragma once
#include <boost/format.hpp>
#include <boost/container/small_vector.hpp>
#include <boost/container/static_vector.hpp>
#include <array>

namespace Core
{
	namespace bst = boost;

	template<class Tp, size_t Sz> using Array = boost::container::static_vector<Tp, Sz>;

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
