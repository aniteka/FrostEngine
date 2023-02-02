#pragma once
#include <GraphicEngineBase.h>


namespace Core
{
	class Window;
	struct GraphicEngineInfo
	{
		Window& renderWindow;
	};

	class GraphicEngineDirectX
		: public GraphicEngineBase
	{
	public:
		GraphicEngineDirectX(const GraphicEngineInfo& createInfo);

		[[nodiscard]] const Window& getRenderWindow() const;
		[[nodiscard]] Window& getRenderWindow();

	protected:
		Window& m_renderWindow;
	};
}

