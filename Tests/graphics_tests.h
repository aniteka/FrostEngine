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

// ��� ���'������ ��'����, ������������� ���� ��������
inline ID3D11Device*			d3d_device = NULL; // �������( ������� ��� ��������� ������� )
inline ID3D11DeviceContext*		immediate_context = NULL; // �������� ��������(���������)
inline IDXGISwapChain*			swap_chain = NULL; // ������ � �������� ���������
// ���� �������� � ���� ������� �� ������ ����� 2 ������
// ������� ����� - �����
// ����� ����� - �� ����� ������� �����

// ��'��� �������� ������
inline ID3D11RenderTargetView*	render_target_view = NULL;

// init of d3d_device, immediate_context, swap_chain
inline HRESULT init_device(const core::window& window)
{
	HRESULT to_ret = S_OK;

	// ������ ����
	UINT width = window.get_width();
	UINT height = window.get_height();
	UINT create_device_flags = 0;

	// ������� ��� ������ ��������
	D3D_DRIVER_TYPE driver_types[] =
	{
		D3D_DRIVER_TYPE_HARDWARE,
		D3D_DRIVER_TYPE_WARP,
		D3D_DRIVER_TYPE_REFERENCE
	};
	UINT num_driver_types = ARRAYSIZE(driver_types);

	// ������ ����� directx, �� ������� ��������
	D3D_FEATURE_LEVEL feature_levels[] =
	{
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_12_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
	};
	UINT num_feature_levels = ARRAYSIZE(feature_levels);

	// ���������, ��� ����� swap_chain
	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.BufferCount = 1;				// 1 ����� �����
	sd.BufferDesc.Width = width;		// ������ ������
	sd.BufferDesc.Height = height;		// ������� ������
	sd.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM; // ������ ������
	sd.BufferDesc.RefreshRate.Numerator = 75; // ������� ������ ������
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT; // ����������� ������
	sd.OutputWindow = window.get_native();
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.Windowed = TRUE; // ������� �����

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

	// ��������� ����� �����
	// render_target_output - ������� �����
	// render_target_view - �����
	ID3D11Texture2D* back_buffer = NULL; // �������� �� ��'��� ������(�� ��, ��������)
	// �������� � back_buffer ������������� ������
	to_ret = swap_chain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&back_buffer);
	if (FAILED(to_ret))
		return to_ret;

	// ��� �� ��� ����� ����������� ���, ����������� ��������������, ������� � back_buffer
	to_ret = d3d_device->CreateRenderTargetView(back_buffer, NULL, &render_target_view);
	// ��������� ��'��� � ���������������� �������� ������ 
	back_buffer->Release();
	if (FAILED(to_ret))
		return to_ret;

	// ��������� ����� ����� � ��������
	immediate_context->OMSetRenderTargets(1, &render_target_view, NULL);

	// ��������� �������
	D3D11_VIEWPORT vp;
	vp.Width = static_cast<FLOAT>(width);
	vp.Height = static_cast<FLOAT>(height);
	vp.MinDepth = 0.f;
	vp.MaxDepth = 1.f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;

	// ��������� ������ �� ��������� ��������
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
	// ���� ����
	float clear_color[4] = { 0.f, 0.f, 1.f, 1.f };
	// ������� �� �������� �����
	immediate_context->ClearRenderTargetView(render_target_view, clear_color);
	// ������ ������ ������
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

