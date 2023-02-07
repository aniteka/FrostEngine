#pragma once
#include <stdexcept>
#include <core/text.h>

namespace core::exceptions
{
	class window_exception : public std::runtime_error
	{
		using base_t = std::runtime_error;
	public:
		using base_t::runtime_error;
		window_exception(const char* str, unsigned errorCode)
			: base_t( fmt::format("{}: {}", str, errorCode) )
		{}
		window_exception(const std::string& str, unsigned errorCode)
			: base_t( fmt::format("{}: {}", str, errorCode) )
		{}
	};
}
