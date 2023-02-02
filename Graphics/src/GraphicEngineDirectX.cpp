#include <GraphicEngineDirectX.h>

#include <Window.h>

#pragma comment (lib, "d3d12.lib")
#include <d3d12.h>

Core::GraphicEngineDirectX::GraphicEngineDirectX(const GraphicEngineInfo& createInfo)
	: m_renderWindow(createInfo.renderWindow)
{

}

const Core::Window& Core::GraphicEngineDirectX::getRenderWindow() const
{
	return m_renderWindow;
}

Core::Window& Core::GraphicEngineDirectX::getRenderWindow()
{
	return m_renderWindow;
}

