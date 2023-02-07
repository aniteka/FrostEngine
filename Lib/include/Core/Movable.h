#pragma once

namespace core
{
	class movable
	{
	public:
		[[nodiscard]] virtual int get_x() const = 0;
		[[nodiscard]] virtual int get_y() const = 0;
		virtual void set_x(int x) = 0;
		virtual void set_y(int y) = 0;
		virtual void set_xy(int x, int y) = 0;
		virtual void move(int dx, int dy) = 0;
	};
}

