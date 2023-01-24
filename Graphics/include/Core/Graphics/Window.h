#pragma once
#include <Core/Transformable.h>
#include <Core/Movable.h>
#include <GLFW/glfw3.h>

#include <optional>
#include <boost/noncopyable.hpp>

namespace Core
{
	class Window
		: public virtual Transformable
		, public virtual Movable
		, boost::noncopyable
	{
	public:
		struct Info;

		Window(const Info& createInfo);
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

		GLFWwindow* getNative();

	private:
		GLFWwindow* m_renderWindow;
	};

	struct Window::Info
	{
		int width = 500;
		int height = 500;
		std::optional<const char*> title;
		GLFWmonitor* monitor = glfwGetPrimaryMonitor();
		std::optional<GLFWwindow*> share;
	};

}
