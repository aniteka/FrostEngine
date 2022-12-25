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
	public:
		graphic_exception(const char* msg, int error_code);
		graphic_exception();

		int get_code() const;
	private:
		int _error_code;
	};
}
