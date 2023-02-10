#include <window.h>

#include <core/exceptions.h>

LRESULT core::window::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	using namespace core;
	window* self = nullptr;

	if (uMsg == WM_NCCREATE)
	{
		auto pCreate = reinterpret_cast<CREATESTRUCT*>(lParam);
		self = static_cast<window*>(pCreate->lpCreateParams);
		SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(self));
	}
	else
		self = reinterpret_cast<window*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));

	return self->window_proc_callback(hwnd, uMsg, wParam, lParam);
}

core::window::window(const info_t& create_info)
{
	static unsigned count_of_windows = 0;
	static std::mutex mutex_of_counter;
	const auto WINDOW_TITLE = create_info.title.value_or(CTEXT("[SET NAME]"));
	mutex_of_counter.lock();
	const auto CLASS_NAME = CFORMAT("{}{}", WINDOW_TITLE, count_of_windows);
	count_of_windows += 1;
	mutex_of_counter.unlock();

	create_and_register_window_class(CLASS_NAME.c_str(), create_info);

	create_window(CLASS_NAME.c_str(), WINDOW_TITLE, create_info);

	// TODO
	ShowWindow(m_render_window.get(), SW_SHOW);
	UpdateWindow(m_render_window.get());
	SetFocus(m_render_window.get());
	SetForegroundWindow(m_render_window.get());
}

core::window::~window() = default;

int core::window::get_width() const
{
	RECT rect;
	if (!GetWindowRect(m_render_window.get(), &rect))
		throw exceptions::window_exception("Exception while getting window rect");
	return rect.right - rect.left;
}

int core::window::get_height() const
{
	RECT rect;
	if (!GetWindowRect(m_render_window.get(), &rect))
		throw exceptions::window_exception("Exception while getting window rect");
	return rect.bottom - rect.top;
}

void core::window::set_width(int width)
{
	std::scoped_lock _(m_window_mutex);
	SetWindowPos(m_render_window.get(), NULL,
		get_x(), get_y(),
		width, get_height(),
		SWP_NOZORDER | SWP_NOACTIVATE);
}

void core::window::set_height(int height)
{
	std::scoped_lock _(m_window_mutex);
	SetWindowPos(m_render_window.get(), NULL,
		get_x(), get_y(),
		get_width(), height,
		SWP_NOZORDER | SWP_NOACTIVATE);
}

void core::window::resize(int width, int height)
{
	std::scoped_lock _(m_window_mutex);
	SetWindowPos(m_render_window.get(), NULL,
		get_x(), get_y(),
		width, height,
		SWP_NOZORDER | SWP_NOACTIVATE);
}

int core::window::get_x() const
{
	RECT rect;
	if (!GetWindowRect(m_render_window.get(), &rect))
		throw exceptions::window_exception("Exception while getting window rect");
	return rect.left;
}

int core::window::get_y() const
{
	RECT rect;
	if (!GetWindowRect(m_render_window.get(), &rect))
		throw exceptions::window_exception("Exception while getting window rect");
	return rect.top;
}

void core::window::set_x(int x)
{
	std::scoped_lock _(m_window_mutex);
	SetWindowPos(m_render_window.get(), NULL,
		x, get_y(),
		0, 0,
		SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);
}

void core::window::set_y(int y)
{
	std::scoped_lock _(m_window_mutex);
	SetWindowPos(m_render_window.get(), NULL,
		get_x(), y,
		0, 0,
		SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);
}

void core::window::set_xy(int x, int y)
{
	std::scoped_lock _(m_window_mutex);
	SetWindowPos(m_render_window.get(), NULL,
		x, y,
		0, 0,
		SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);
}

void core::window::move(int dx, int dy)
{
	std::scoped_lock _(m_window_mutex);
	set_xy(get_x() + dx,
		get_y() + dy);
}

void core::window::close()
{
	if (m_should_close == window_close_state::closed)
		throw exceptions::window_exception("The window is closed");
	m_should_close = window_close_state::should_close;
}

bool core::window::should_close()
{
	switch (m_should_close)
	{
	[[likely]] case window_close_state::unclosed:
		return false;
	case window_close_state::should_close:
		{
		auto window_handler = m_render_window.release();
		if (DestroyWindow(window_handler))
			throw exceptions::window_exception("Can not close the window", GetLastError());
		}
		[[fallthrough]];
	case window_close_state::closed:
		return true;
	}
	return false;
}

core::window::window_close_state core::window::close_state() const
{
	return m_should_close;
}

core::window::native_window_t* core::window::get_native() const
{
	return m_render_window.get();
}

void core::window::create_and_register_window_class(ctext_t CLASS_NAME, [[maybe_unused]]const core::window::info_t& create_info)
{
	auto instance = GetModuleHandle(NULL);
	WNDCLASSEX wc = { };
	wc.cbSize = sizeof WNDCLASSEX;

	wc.lpfnWndProc = WindowProc;
	wc.hInstance = instance;
	wc.lpszClassName = CLASS_NAME;
	wc.lpszMenuName = 0; // TODO

	wc.style = CS_HREDRAW | CS_VREDRAW; // TODO
	wc.hIcon = LoadIcon(instance, IDI_APPLICATION); // TODO
	wc.hIconSm = LoadIcon(instance, IDI_APPLICATION); // TODO
	wc.hCursor = LoadCursor(instance, IDC_ARROW); // TODO
	wc.hbrBackground = static_cast<HBRUSH>(GetStockObject(WHITE_BRUSH)); // TODO

	wc.cbClsExtra = 0; // TODO
	wc.cbWndExtra = 0; // TODO

	if(!RegisterClassEx(&wc))
	{
		UnregisterClass(CLASS_NAME, instance);
		throw exceptions::window_exception("Class register exception", GetLastError());
	}
}

void core::window::create_window(ctext_t CLASS_NAME, ctext_t WINDOW_TITLE, const info_t& create_info)
{
	auto instance = GetModuleHandle(NULL);

	m_render_window.reset(CreateWindowEx(
		0,                              
		CLASS_NAME,                     
		WINDOW_TITLE,
		WS_OVERLAPPEDWINDOW,            
		CW_USEDEFAULT, CW_USEDEFAULT, 
		create_info.width, create_info.height,
		nullptr,
		nullptr,  
		instance,
		this	
	));

	if (!m_render_window)
	{
		UnregisterClass(CLASS_NAME, instance);
		throw exceptions::window_exception("Exception while creating window", GetLastError());
	}
}

LRESULT core::window::window_proc_callback(HWND window_handler, UINT msg, WPARAM wparam, LPARAM lparam)
{
	switch (msg)
	{
	case WM_DESTROY:
		this->close();
		return 0;
	}

	return DefWindowProc(window_handler, msg, wparam, lparam);
}

