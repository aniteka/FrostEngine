#include <core/graphics/window_event_manager.h>
#include <core/graphics/Window.h>

core::window_events core::window_event_manager::get_current_event() const
{
	return m_current_event;
}

void core::window_event_manager::update() const
{
	MSG msg;
	ZeroMemory(&msg, sizeof(msg));
	if(PeekMessage(&msg, m_window.get_native(), 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}
