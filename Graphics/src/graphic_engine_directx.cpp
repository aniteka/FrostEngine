#include <graphic_engine_directx.h>

#include <Window.h>

#pragma comment (lib, "d3d12.lib")
#include <d3d12.h>

core::graphic_engine_directx::graphic_engine_directx(const graphic_engine_info& createInfo)
	: m_render_window(createInfo.render_window)
{
	
}

const core::window& core::graphic_engine_directx::get_render_window() const
{
	return m_render_window;
}

core::window& core::graphic_engine_directx::get_render_window()
{
	return m_render_window;
}

