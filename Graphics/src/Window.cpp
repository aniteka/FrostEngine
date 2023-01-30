#include <window.h>

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
	auto instance = GetModuleHandle(NULL);
	const TEXT_TYPE CLASS_NAME = TEXT("Sample Window Class");

	WNDCLASSEX wc = { };
	wc.cbSize = sizeof WNDCLASSEX;
	wc.lpfnWndProc = WindowProc;
	wc.hInstance = instance;
	wc.lpszClassName = CLASS_NAME;

	if(!RegisterClassEx(&wc))
	{
		// TODO exceptions
		std::terminate();
	}

	m_renderWindow = CreateWindowEx(
		0,                              
		CLASS_NAME,                     
		createInfo.title.value_or(TEXT("[SET NAME]")),
		WS_OVERLAPPEDWINDOW,            
		CW_USEDEFAULT, CW_USEDEFAULT, 
		createInfo.width, createInfo.height,
		NULL,
		NULL,  
		instance,
		NULL
	);

	if (m_renderWindow == NULL)
	{
		// TODO exceptions
		std::terminate();
	}

	ShowWindow(m_renderWindow, SW_SHOW);
}

Core::Window::~Window()
{
	DestroyWindow(m_renderWindow);
	m_renderWindow = nullptr;
}

int Core::Window::getWidth() const
{
	RECT rect;
	if(!GetWindowRect(m_renderWindow, &rect))
	{
		// TODO exception
		std::terminate();
	}
	return rect.right - rect.left;
}

int Core::Window::getHeight() const
{
	RECT rect;
	if(!GetWindowRect(m_renderWindow, &rect))
	{
		// TODO exception
		std::terminate();
	}
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
	if(!GetWindowRect(m_renderWindow, &rect))
	{
		// TODO exception
		std::terminate();
	}
	return rect.left;
}

int Core::Window::getY() const
{
	RECT rect;
	if(!GetWindowRect(m_renderWindow, &rect))
	{
		// TODO exception
		std::terminate();
	}
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
	return false;
}

Core::Window::NativeWindowType Core::Window::getNative()
{
	return m_renderWindow;
}
