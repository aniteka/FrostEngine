#pragma once

namespace Core
{
	class Movable
	{
	public:
		[[nodiscard]] virtual int getX() const = 0;
		[[nodiscard]] virtual int getY() const = 0;
		virtual void setX(int x) = 0;
		virtual void setY(int y) = 0;
		virtual void setXY(int x, int y) = 0;
		virtual void move(int dx, int dy) = 0;
	};
}

