#pragma once
#include <boost/noncopyable.hpp>

#ifdef WIN32
#pragma comment(lib, "winmm.lib")
#include <core/text.h>
#include <windows.h>
#endif

namespace core
{
	class window;

	enum class window_events : unsigned
	{
		none = WM_NULL,
		destroy = WM_DESTROY
	};

	class window_event_manager final
		: boost::noncopyable
	{
		explicit window_event_manager(const core::window& window)
			: m_window(window)
		{}

	public:
		[[nodiscard]] window_events get_current_event() const;
		void update() const;

		friend class window;
	protected:
		window_events m_current_event = window_events::none;
		const core::window& m_window;
	};
}
