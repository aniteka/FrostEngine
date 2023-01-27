#pragma once
#include <Core/Graphics/Window.h>
#include <boost/noncopyable.hpp>
#include <atomic>

#include <boost/smart_ptr/intrusive_ptr.hpp>

namespace Core
{
	class GraphicEngineOpenGL
		: boost::noncopyable
	{
		using WindowType = Window;
		using WindowPtr = boost::intrusive_ptr<WindowType>;
	public:
		struct Info;

		explicit GraphicEngineOpenGL(const Info& createInfo);
		~GraphicEngineOpenGL();
		GraphicEngineOpenGL(GraphicEngineOpenGL&& other) = default;
		GraphicEngineOpenGL& operator=(GraphicEngineOpenGL&& other) = default;

		WindowPtr initWindow(Window::Info createInfo);
		[[nodiscard]] WindowPtr getWindow() const;

		void update(unsigned dt);

	protected:
		WindowPtr m_window;
	};

	struct GraphicEngineOpenGL::Info
	{
		int major = 2, minor = 0;
	};
}
