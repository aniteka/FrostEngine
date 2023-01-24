#pragma once

namespace Core
{
	class Transformable
	{
	public:
		[[nodiscard]] virtual int getWidth() const = 0;
		[[nodiscard]] virtual int getHeight() const = 0;
		virtual void setWidth(int width) = 0;
		virtual void setHeight(int height) = 0;
		virtual void resize(int width, int height) = 0;
	};
}

