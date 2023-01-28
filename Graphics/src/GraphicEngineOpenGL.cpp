#include "GraphicEngineOpenGL.h"

#include <exception>
#include <Core/Graphics/Window.h>

Core::GraphicEngineOpenGL::SelfPtrType Core::GraphicEngineOpenGL::m_self = nullptr;

Core::GraphicEngineOpenGL::GraphicEngineOpenGL(const Info& createInfo)
{
	if(!glfwInit())
	{
		// TODO exception
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, createInfo.major);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, createInfo.minor);
}

Core::GraphicEngineOpenGL::~GraphicEngineOpenGL()
{
	delete m_window;
	glfwTerminate();
}

Core::GraphicEngineOpenGL::WindowPtrType Core::GraphicEngineOpenGL::initWindow(const WindowInfoType& createInfo)
{
	try
	{
		m_window = new Window(createInfo);
		glfwMakeContextCurrent(m_window->getNative());
	}
	catch (...)
	{
		// TODO exception 

		std::rethrow_exception(std::current_exception());
	}

	return m_window;
}

Core::GraphicEngineOpenGL::WindowPtrType Core::GraphicEngineOpenGL::getWindow() const
{
	return m_window;
}

void Core::GraphicEngineOpenGL::setErrorCallback(const ErrorCallbackType& callback)
{
	glfwSetErrorCallback(callback);
}

void Core::GraphicEngineOpenGL::update(unsigned dt)
{
	glfwPollEvents();
}

Core::GraphicEngineOpenGL::SelfPtrType Core::GraphicEngineOpenGL::get()
{
	// TODO thread save 
	if(m_self == nullptr)
	{
		// TODO exception
	}
	return m_self;
}

Core::GraphicEngineOpenGL::SelfPtrType Core::GraphicEngineOpenGL::get(const GraphicEngineOpenGLInfo& createInfo)
{
	// TODO thread save 
	if(m_self == nullptr)
	{
		m_self = new GraphicEngineOpenGL{ createInfo };
	}
	return m_self;
}

