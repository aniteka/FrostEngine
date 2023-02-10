#pragma once
#include <stdexcept>
#include <core/text.h>

namespace core::exceptions
{
	class window_exception : public std::runtime_error
	{
		using base_t = std::runtime_error;
		unsigned m_error_code = 0;
	public:
		using base_t::runtime_error;
		window_exception(const char* str, unsigned error_code)
			: base_t( fmt::format("{}: {}", str, error_code) )
			, m_error_code(error_code)
		{}
		window_exception(const std::string& str, unsigned error_code)
			: base_t( fmt::format("{}: {}", str, error_code) )
			, m_error_code(error_code)
		{}

		unsigned error_code() const
		{
			return m_error_code;
		}
	};
}
