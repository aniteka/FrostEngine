#include "GraphicEngineOpenGL.h"

#include <exception>
#include <Core/Graphics/Window.h>

Core::GraphicEngineOpenGL::SelfPtrType Core::GraphicEngineOpenGL::m_self = nullptr;

Core::GraphicEngineOpenGL::GraphicEngineOpenGL([[maybe_unused]]const Info& createInfo)
{
}

Core::GraphicEngineOpenGL::~GraphicEngineOpenGL()
{
	delete m_window;
}

Core::GraphicEngineOpenGL::WindowPtrType Core::GraphicEngineOpenGL::initWindow(const WindowInfoType& createInfo)
{
	try
	{
		m_window = new Window(createInfo);
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

void Core::GraphicEngineOpenGL::update([[maybe_unused]]unsigned dt)
{
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

