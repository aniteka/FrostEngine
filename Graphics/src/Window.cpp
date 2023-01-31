#include <window.h>

#include <atomic>
#include <Core/Exceptions.h>

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

Core::Window::Window(const Info& createInfo)
{
	static std::atomic_uint countOfWindows = 0;
	const auto WINDOW_TITLE = createInfo.title.value_or(TEXT("[SET NAME]"));
	const auto CLASS_NAME = (StringStreamType{} << WINDOW_TITLE << countOfWindows).str();

	auto instance = GetModuleHandle(NULL);

	WNDCLASSEX wc = { };
	wc.cbSize = sizeof WNDCLASSEX;
	wc.lpfnWndProc = WindowProc;
	wc.hInstance = instance;
	wc.lpszClassName = CLASS_NAME.c_str();

	if(!RegisterClassEx(&wc))
	{
		UnregisterClass(CLASS_NAME.c_str(), instance);
		throw Exceptions::WindowException("Class register exception", GetLastError());
	}

	m_renderWindow = CreateWindowEx(
		0,                              
		CLASS_NAME.c_str(),                     
		WINDOW_TITLE,
		WS_OVERLAPPEDWINDOW,            
		CW_USEDEFAULT, CW_USEDEFAULT, 
		createInfo.width, createInfo.height,
		nullptr,
		nullptr,  
		instance,
		nullptr	
	);

	if (m_renderWindow == nullptr)
	{
		UnregisterClass(CLASS_NAME.c_str(), instance);
		throw Exceptions::WindowException("Exception while creating window", GetLastError());
	}
	countOfWindows += 1;

	// TODO
	ShowWindow(m_renderWindow, SW_SHOW);
	UpdateWindow(m_renderWindow);
	SetFocus(m_renderWindow);
	SetForegroundWindow(m_renderWindow);
}

Core::Window::~Window()
{
	DestroyWindow(m_renderWindow);
	m_renderWindow = nullptr;
}

int Core::Window::getWidth() const
{
	RECT rect;
	if (!GetWindowRect(m_renderWindow, &rect))
		throw Exceptions::WindowException("Exception while getting window rect");
	return rect.right - rect.left;
}

int Core::Window::getHeight() const
{
	RECT rect;
	if (!GetWindowRect(m_renderWindow, &rect))
		throw Exceptions::WindowException("Exception while getting window rect");
	return rect.bottom - rect.top;
}

void Core::Window::setWidth(int width)
{
	SetWindowPos(m_renderWindow, NULL,
		getX(), getY(),
		width, getHeight(),
		SWP_NOZORDER | SWP_NOACTIVATE);
}

void Core::Window::setHeight(int height)
{
	SetWindowPos(m_renderWindow, NULL,
		getX(), getY(),
		getWidth(), height,
		SWP_NOZORDER | SWP_NOACTIVATE);
}

void Core::Window::resize(int width, int height)
{
	SetWindowPos(m_renderWindow, NULL,
		getX(), getY(),
		width, height,
		SWP_NOZORDER | SWP_NOACTIVATE);
}

int Core::Window::getX() const
{
	RECT rect;
	if (!GetWindowRect(m_renderWindow, &rect))
		throw Exceptions::WindowException("Exception while getting window rect");
	return rect.left;
}

int Core::Window::getY() const
{
	RECT rect;
	if (!GetWindowRect(m_renderWindow, &rect))
		throw Exceptions::WindowException("Exception while getting window rect");
	return rect.top;
}

void Core::Window::setX(int x)
{
	SetWindowPos(m_renderWindow, NULL,
		x, getY(),
		0, 0,
		SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);
}

void Core::Window::setY(int y)
{
	SetWindowPos(m_renderWindow, NULL,
		getX(), y,
		0, 0,
		SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);
}

void Core::Window::setXY(int x, int y)
{
	SetWindowPos(m_renderWindow, NULL,
		x, y,
		0, 0,
		SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);
}

void Core::Window::move(int dx, int dy)
{
	setXY(getX() + dx,
		getY() + dy);
}

bool Core::Window::shouldClose() const
{
	// TODO
	return false;
}

Core::Window::NativeWindowType Core::Window::getNative()
{
	return m_renderWindow;
}

