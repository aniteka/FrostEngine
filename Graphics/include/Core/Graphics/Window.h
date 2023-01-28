#pragma once
#include <Core/Transformable.h>
#include <Core/Movable.h>
#include <boost/noncopyable.hpp>

#include <optional>
#include <GLFW/glfw3.h>

namespace Core
{
	struct WindowInfo;

	class Window final
		: public virtual Transformable
		, public virtual Movable
		, boost::noncopyable
	{
	public:
		using Info = WindowInfo;

		explicit Window(const Info& createInfo);
		virtual ~Window();
		Window(Window&&) noexcept = default;
		Window& operator=(Window&&) noexcept = default;

		[[nodiscard]] int getWidth() const override;
		[[nodiscard]] int getHeight() const override;
		void setWidth(int width) override;
		void setHeight(int height) override;
		void resize(int width, int height) override;
		[[nodiscard]] int getX() const override;
		[[nodiscard]] int getY() const override;
		void setX(int x) override;
		void setY(int y) override;
		void setXY(int x, int y) override;
		void move(int dx, int dy) override;

		bool shouldClose() const;

		GLFWwindow* getNative();
	private:
		GLFWwindow* m_renderWindow;
	};

	struct WindowInfo
	{
		int width = 500;
		int height = 500;
		std::optional<const char*> title;
		std::optional<GLFWmonitor*> monitor;
		std::optional<GLFWwindow*> share;
	};

}
