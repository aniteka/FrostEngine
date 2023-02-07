#pragma once
#include "graphic_engine_base.h" 


namespace core
{
	class window;
	struct graphic_engine_info
	{
		window& render_window;
	};

	class graphic_engine_directx
		: public graphic_engine_base
	{
	public:
		graphic_engine_directx(const graphic_engine_info& createInfo);

		[[nodiscard]] const window& get_render_window() const;
		[[nodiscard]] window& get_render_window();

	protected:
		window& m_render_window;
	};
}

