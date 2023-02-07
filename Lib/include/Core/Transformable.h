#pragma once

namespace core
{
	class transformable
	{
	public:
		[[nodiscard]] virtual int get_width() const = 0;
		[[nodiscard]] virtual int get_height() const = 0;
		virtual void set_width(int width) = 0;
		virtual void set_height(int height) = 0;
		virtual void resize(int width, int height) = 0;
	};
}

