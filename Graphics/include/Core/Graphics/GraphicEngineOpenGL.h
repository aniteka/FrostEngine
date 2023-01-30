#pragma once
#include <Core/Graphics/GraphicEngineBase.h>
#include <boost/noncopyable.hpp>
#include <boost/smart_ptr/intrusive_ref_counter.hpp>
#include <boost/smart_ptr/intrusive_ptr.hpp>

namespace Core
{
	class Window;
	struct WindowInfo;
	struct GraphicEngineOpenGLInfo;

	class GraphicEngineOpenGL
		: public GraphicEngineBase
		, protected boost::noncopyable
		, public boost::intrusive_ref_counter<GraphicEngineOpenGL>
	{
		using SelfType = GraphicEngineOpenGL;
		using SelfPtrType = boost::intrusive_ptr<SelfType>;
		using WindowType = Window;
		using WindowInfoType = WindowInfo;
		using WindowPtrType = WindowType*;
		using ErrorCallbackType = void (*)(int, const char*);

	public:
		using Info = GraphicEngineOpenGLInfo;

	private: explicit GraphicEngineOpenGL(const Info& createInfo);
	public:
		~GraphicEngineOpenGL();
		GraphicEngineOpenGL(GraphicEngineOpenGL&& other) = default;
		GraphicEngineOpenGL& operator=(GraphicEngineOpenGL&& other) = default;

		WindowPtrType initWindow(const WindowInfoType& createInfo);
		[[nodiscard]] WindowPtrType getWindow() const;

		void update(unsigned dt);

		static SelfPtrType get();
		static SelfPtrType get(const GraphicEngineOpenGLInfo& createInfo);

	protected:
		WindowPtrType m_window = nullptr;

		static SelfPtrType m_self;
	};

	struct GraphicEngineOpenGLInfo
	{
		int major = 2, minor = 0;
	};
}
