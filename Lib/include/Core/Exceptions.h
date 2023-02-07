#pragma once
#include <stdexcept>
#include <fmt/core.h>

namespace Core::Exceptions
{
	class WindowException : public std::runtime_error
	{
		using BaseType = std::runtime_error;
	public:
		using BaseType::runtime_error;
		WindowException(const char* str, unsigned errorCode)
			: BaseType( fmt::format("{}: {}", str, errorCode) )
		{}
		WindowException(const std::string& str, unsigned errorCode)
			: BaseType( fmt::format("{}: {}", str, errorCode) )
		{}
	};
}
