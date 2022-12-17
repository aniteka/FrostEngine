#pragma once

#include <stdexcept>

namespace Core
{
	class Graphics
	{
	public:
		static void test();
	};


	class graphic_exception : public std::runtime_error
	{
		int _error_code;
	public:
		graphic_exception(const char* msg, int error_code);

		int get_code() const;
	};
}
