#pragma once
#include <stdexcept>
#include <boost/format.hpp>

namespace Core::Exceptions
{
	using ExceptionFormat = boost::basic_format<char>;

	class WindowException : public std::runtime_error
	{
		using BaseType = std::runtime_error;
	public:
		using BaseType::runtime_error;
		WindowException(const char* str, unsigned errorCode)
			: BaseType( (ExceptionFormat("%1%: %2%") % str % errorCode).str().c_str() )
		{}
		WindowException(const std::string& str, unsigned errorCode)
			: BaseType( (ExceptionFormat("%1%: %2%") % str % errorCode).str().c_str() )
		{}
	};
}
