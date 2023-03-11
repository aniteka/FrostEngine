#pragma once
#include <stdexcept>
#include <core/text.h>

namespace core::exceptions
{
	class runtime_error_with_code : public std::runtime_error
	{
		using base_t = std::runtime_error;
	public:
		using base_t::runtime_error;
		using base_t::operator=;
		runtime_error_with_code(const char* str, unsigned error_code)
			: base_t(fmt::format("{}: {}", str, error_code))
		{}
		runtime_error_with_code(const std::string& str, unsigned error_code)
			: base_t(fmt::format("{}: {}", str, error_code))
		{}
	};

	class window_exception final : public runtime_error_with_code
	{
		using base_t = runtime_error_with_code;
	public:
		using base_t::runtime_error_with_code;
		using base_t::operator=;
	};

	class graphics_exception final : public runtime_error_with_code 
	{
		using base_t = runtime_error_with_code;
	public:
		using base_t::runtime_error_with_code;
		using base_t::operator=;
	};
}
