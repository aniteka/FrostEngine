#include <Window.h>

Core::Window::Window(const Info& createInfo)
{
	m_renderWindow = glfwCreateWindow(
		createInfo.width,
		createInfo.height,
		createInfo.title.value_or("[Set name]"),
		createInfo.monitor,
		createInfo.share.value_or(nullptr));
}

Core::Window::~Window()
{
	glfwDestroyWindow(m_renderWindow);
}

int Core::Window::getWidth() const
{
	int width, height;
	glfwGetWindowSize(m_renderWindow, &width, &height);
	return width;
}

int Core::Window::getHeight() const
{
	int width, height;
	glfwGetWindowSize(m_renderWindow, &width, &height);
	return height;
}

void Core::Window::setWidth(int width)
{
	glfwSetWindowSize(m_renderWindow, width, getHeight());
}

void Core::Window::setHeight(int height)
{
	glfwSetWindowSize(m_renderWindow, getWidth(), height);
}

void Core::Window::resize(int width, int height)
{
	glfwSetWindowSize(m_renderWindow, width, height);
}

int Core::Window::getX() const
{
	int x, y;
	glfwGetWindowPos(m_renderWindow, &x, &y);
	return x;
}

int Core::Window::getY() const
{
	int x, y;
	glfwGetWindowPos(m_renderWindow, &x, &y);
	return y;
}

void Core::Window::setX(int x)
{
	glfwSetWindowPos(m_renderWindow, x, getY());
}

void Core::Window::setY(int y)
{
	glfwSetWindowPos(m_renderWindow, getX(), y);
}

void Core::Window::setXY(int x, int y)
{
	glfwSetWindowPos(m_renderWindow, x, y);
}

void Core::Window::move(int dx, int dy)
{
	setXY(getX() + dx,
		getY() + dy);
}

GLFWwindow* Core::Window::getNative()
{
	return m_renderWindow;
}
