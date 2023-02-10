#pragma once

#include <core/graphics/window.h>
#include <core/log.h>
CREATE_LOGGER(grapgics_test_logger);

#include <boost/test/unit_test.hpp>

#include <d3d11.h>
#include <d3dx11.h>

BOOST_AUTO_TEST_SUITE(graphics_tests)

BOOST_AUTO_TEST_CASE(window_test)
{
	using namespace core;
	auto main_window = window({
		.width = 500,
		.height = 600,
		.title = CTEXT("Test") });

	main_window.set_xy(1, 2);
	BOOST_CHECK_EQUAL(main_window.get_x(), 1);
	BOOST_CHECK_EQUAL(main_window.get_y(), 2);

	main_window.move(100, 200);
	BOOST_CHECK_EQUAL(main_window.get_x(), 101);
	BOOST_CHECK_EQUAL(main_window.get_y(), 202);

	main_window.set_x(10);
	main_window.set_y(20);
	BOOST_CHECK_EQUAL(main_window.get_x(), 10);
	BOOST_CHECK_EQUAL(main_window.get_y(), 20);

	BOOST_CHECK_EQUAL(main_window.get_width(), 500);
	BOOST_CHECK_EQUAL(main_window.get_height(), 600);

	main_window.resize(200, 300);
	BOOST_CHECK_EQUAL(main_window.get_width(), 200);
	BOOST_CHECK_EQUAL(main_window.get_height(), 300);

	main_window.set_width(800);
	main_window.set_height(800);
	BOOST_CHECK_EQUAL(main_window.get_width(), 800);
	BOOST_CHECK_EQUAL(main_window.get_height(), 800);

	//MSG msg = { };
	//while (GetMessage(&msg, mainWindow.getNative(), 0, 0) > 0)
	//{
	//	TranslateMessage(&msg);
	//	DispatchMessage(&msg);
	//}
}



inline D3D_DRIVER_TYPE			driver_type = D3D_DRIVER_TYPE_NULL; // GPU or CPU
inline D3D_FEATURE_LEVEL		feature_level = D3D_FEATURE_LEVEL_11_0; // DirectX version

// Три пов'язаних об'єкти, ініціалізуються однією функцією
inline ID3D11Device*			d3d_device = NULL; // Пристрій( Потрібен для створення ресурсів )
inline ID3D11DeviceContext*		immediate_context = NULL; // Контекст пристрою(малювання)
inline IDXGISwapChain*			swap_chain = NULL; // працює з буферами малювання
// буде існувати в любій програмі та містить мінімум 2 буфери
// передній буфер - екран
// задній буфер - на ньому малюємо сцену

// об'єкт заднього буферу
inline ID3D11RenderTargetView*	render_target_view = NULL;

// init of d3d_device, immediate_context, swap_chain
inline HRESULT init_device(const core::window& window)
{
	HRESULT to_ret = S_OK;

	// розміри вікна
	UINT width = window.get_width();
	UINT height = window.get_height();
	UINT create_device_flags = 0;

	// потрібен для вибору пристрою
	D3D_DRIVER_TYPE driver_types[] =
	{
		D3D_DRIVER_TYPE_HARDWARE,
		D3D_DRIVER_TYPE_WARP,
		D3D_DRIVER_TYPE_REFERENCE
	};
	UINT num_driver_types = ARRAYSIZE(driver_types);

	// список версій directx, які підтримує програма
	D3D_FEATURE_LEVEL feature_levels[] =
	{
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_12_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
	};
	UINT num_feature_levels = ARRAYSIZE(feature_levels);

	// структура, яка описує swap_chain
	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.BufferCount = 1;				// 1 задній буфер
	sd.BufferDesc.Width = width;		// ширина буфера
	sd.BufferDesc.Height = height;		// довжина буфера
	sd.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM; // формат пікселя
	sd.BufferDesc.RefreshRate.Numerator = 75; // частота обнови екрана
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT; // призначення буферу
	sd.OutputWindow = window.get_native();
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.Windowed = TRUE; // віконний режим

	for(UINT driver_type_index = 0; driver_type_index < num_driver_types; driver_type_index++)
	{
		driver_type = driver_types[driver_type_index];
		to_ret = D3D11CreateDeviceAndSwapChain(
			NULL,
			driver_type,
			NULL,
			create_device_flags,
			feature_levels,
			num_feature_levels,
			D3D11_SDK_VERSION,
			&sd,
			&swap_chain,
			&d3d_device,
			&feature_level,
			&immediate_context
		);
		if (SUCCEEDED(to_ret))
			break;
	}
	if (FAILED(to_ret)) 
		return to_ret;

	// створюємо задній буфер
	// render_target_output - передній буфер
	// render_target_view - задній
	ID3D11Texture2D* back_buffer = NULL; // вказівник на об'єкт буфера(да да, текстура)
	// загрузка в back_buffer характеристик буфера
	to_ret = swap_chain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&back_buffer);
	if (FAILED(to_ret))
		return to_ret;

	// сам по собі буфер створюється тут, використовує характеристики, передані в back_buffer
	to_ret = d3d_device->CreateRenderTargetView(back_buffer, NULL, &render_target_view);
	// звільняємо об'єкт з характеристиками заднього буфера 
	back_buffer->Release();
	if (FAILED(to_ret))
		return to_ret;

	// підключаємо задній буфер в контекст
	immediate_context->OMSetRenderTargets(1, &render_target_view, NULL);

	// настройка вюпорта
	D3D11_VIEWPORT vp;
	vp.Width = static_cast<FLOAT>(width);
	vp.Height = static_cast<FLOAT>(height);
	vp.MinDepth = 0.f;
	vp.MaxDepth = 1.f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;

	// підключаємо вюпорт до контексту пристрою
	immediate_context->RSSetViewports(1, &vp);

	return S_OK;
}
inline void cleanup_device()
{
	if (immediate_context) 
		immediate_context->ClearState();
	if (render_target_view)
		render_target_view->Release();
	if (swap_chain)
		swap_chain->Release();
	if (immediate_context)
		immediate_context->Release();
	if (d3d_device)
		d3d_device->Release();
}
inline void render()
{
	// синій колір
	float clear_color[4] = { 0.f, 0.f, 1.f, 1.f };
	// малюємо на задньому екрані
	immediate_context->ClearRenderTargetView(render_target_view, clear_color);
	// міняємо місцями буфери
	swap_chain->Present(0, 0);
}

BOOST_AUTO_TEST_CASE(graphics_test)
{
	core::window window({
		.width = 500,
		.height = 400,
		.title = CTEXT("Hello Window")});

	auto err = init_device(window);
	if(FAILED(err))
	{
		cleanup_device();
		grapgics_test_logger->critical("can't init device: {}", err);
		std::exit(-1);
	}

	MSG msg;
	ZeroMemory(&msg, sizeof(MSG));
	while(!window.should_close())
	{
		if (PeekMessage(&msg, window.get_native(), 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
			render();
	}
	cleanup_device();
}

BOOST_AUTO_TEST_SUITE_END()

