#include "GraphicEngineOpenGL.h"
#include <exception>
#include <boost/log/trivial.hpp>

Core::GraphicEngineOpenGL::GraphicEngineOpenGL(const Info& createInfo)
{
	if(!glfwInit())
	{
		// TODO exception
		BOOST_LOG_TRIVIAL(fatal) << "Can't init glfw";
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, createInfo.major);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, createInfo.minor);
}

Core::GraphicEngineOpenGL::~GraphicEngineOpenGL()
{
	m_window.reset();
	glfwTerminate();
}

Core::GraphicEngineOpenGL::WindowPtr Core::GraphicEngineOpenGL::initWindow(Window::Info createInfo)
{
	try
	{
		m_window = new Window(createInfo);
		glfwMakeContextCurrent(m_window.get()->getNative());
	}
	catch (...)
	{
		using namespace boost::log;
		// TODO exception 
		BOOST_LOG_TRIVIAL(error) << "Create window exception";

		std::rethrow_exception(std::current_exception());
	}

	return m_window;
}

Core::GraphicEngineOpenGL::WindowPtr Core::GraphicEngineOpenGL::getWindow() const
{
	return m_window;
}

void Core::GraphicEngineOpenGL::update(unsigned dt)
{
	glfwPollEvents();
}
