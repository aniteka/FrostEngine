#pragma once

#include <core/graphics/window.h>
#include <core/log.h>
CREATE_LOGGER(grapgics_test_logger);

#include <boost/test/unit_test.hpp>

#include <d3d11.h>
#include <d3dx11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>

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

struct SimpleVertex
{
	DirectX::XMFLOAT3 pos;
};
template<typename Type, size_t Size>
using array_t = Type[Size];

inline D3D_DRIVER_TYPE			driver_type = D3D_DRIVER_TYPE_NULL;
inline D3D_FEATURE_LEVEL		feature_level = D3D_FEATURE_LEVEL_11_0;
inline ID3D11Device*			d3d_device = NULL;
inline ID3D11DeviceContext*		immediate_context = NULL; 
inline IDXGISwapChain*			swap_chain = NULL;
inline ID3D11RenderTargetView*	render_target_view = NULL;
// Вершинний шейдер
inline ID3D11VertexShader*		vertex_shader = NULL;
// Піксельний шейдер
inline ID3D11PixelShader*		pixel_shader = NULL;
// Опис формату вершин
inline ID3D11InputLayout*		vertex_layout = NULL;
// Буфер вершин
inline ID3D11Buffer*			vertex_buffer = NULL;

// Загружає та компілює шейдери 
inline HRESULT compile_shader_from_file( const WCHAR* file_name, LPCSTR entry_point, LPCSTR shader_model, ID3DBlob** blob_out )
{
	HRESULT to_ret = S_OK;
	DWORD shader_flags = D3DCOMPILE_ENABLE_STRICTNESS;
	ID3DBlob* error_blob;
	to_ret = D3DX11CompileFromFile(
		file_name,
		NULL,
		NULL,
		entry_point,
		shader_model,
		shader_flags,
		0,
		NULL,
		blob_out,
		&error_blob,
		NULL);

	if (FAILED(to_ret) && error_blob)
		grapgics_test_logger->error("%s", static_cast<const char*>(error_blob->GetBufferPointer()));
	if (error_blob)
		error_blob->Release();

	return to_ret;
}
inline HRESULT init_device(const core::window& window)
{
	HRESULT to_ret = S_OK;

	UINT width = window.get_width();
	UINT height = window.get_height();
	UINT create_device_flags = 0;
#ifndef NDEBUG
	create_device_flags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	D3D_DRIVER_TYPE driver_types[] =
	{
		D3D_DRIVER_TYPE_HARDWARE,
		D3D_DRIVER_TYPE_WARP,
		D3D_DRIVER_TYPE_REFERENCE
	};
	UINT num_driver_types = ARRAYSIZE(driver_types);

	D3D_FEATURE_LEVEL feature_levels[] =
	{
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_12_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
	};
	UINT num_feature_levels = ARRAYSIZE(feature_levels);

	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.BufferCount = 1;	
	sd.BufferDesc.Width = width;
	sd.BufferDesc.Height = height;
	sd.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 75;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow = window.get_native();
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.Windowed = TRUE;

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

	ID3D11Texture2D* back_buffer = NULL; 
	to_ret = swap_chain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&back_buffer);
	if (FAILED(to_ret))
		return to_ret;

	to_ret = d3d_device->CreateRenderTargetView(back_buffer, NULL, &render_target_view);
	back_buffer->Release();
	if (FAILED(to_ret))
		return to_ret;

	immediate_context->OMSetRenderTargets(1, &render_target_view, NULL);

	D3D11_VIEWPORT vp;
	vp.Width = static_cast<FLOAT>(width);
	vp.Height = static_cast<FLOAT>(height);
	vp.MinDepth = 0.f;
	vp.MaxDepth = 1.f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;

	immediate_context->RSSetViewports(1, &vp);

	return S_OK;
}
// Завантажуємо шейдери та створюємо буфер вершин
inline HRESULT init_geometry()
{
	HRESULT to_ret = S_OK;

	// Створення вершинного шейдеру

	// Допоміжний об'єкт, просто місце в пам'яті, буде зберігатися скомпільований вершинний шейдер
	ID3DBlob* vsblob = NULL;
	to_ret = compile_shader_from_file(L"shader.fx", "VS", "vs_4_0", &vsblob);
	if(FAILED(to_ret))
	{
		grapgics_test_logger->critical("cannot compile FX file");
		return to_ret;
	}

	to_ret = d3d_device->CreateVertexShader(
		vsblob->GetBufferPointer(),
		vsblob->GetBufferSize(),
		NULL,
		&vertex_shader
	);
	if(FAILED(to_ret))
	{
		vsblob->Release();
		return to_ret;
	}

	// Визначає шаблон вершин
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{	
			"POSITION", // семантичне ім'я
			0, // семантичний індекс
			DXGI_FORMAT_R32G32B32_FLOAT, // розмір(RGB тут XYZ)
			0, // вхідний слот
			0, // адрес початку данних в буфері вершин
			D3D11_INPUT_PER_VERTEX_DATA, // неважно
			0 // неважно
		}
	};

	UINT num_elements = ARRAYSIZE(layout);

	// створення шаблону вершин
	to_ret = d3d_device->CreateInputLayout(
		layout,
		num_elements,
		vsblob->GetBufferPointer(),
		vsblob->GetBufferSize(),
		&vertex_layout);
	vsblob->Release();

	if (FAILED(to_ret))
		return to_ret;

	// підключення шаблону
	immediate_context->IASetInputLayout(vertex_layout);

	// створення піксельного шейдеру
	ID3DBlob* psblob = NULL;
	to_ret = compile_shader_from_file(L"shader.fx", "PS", "ps_4_0", &psblob);
	if(FAILED(to_ret))
	{
		grapgics_test_logger->critical("cannot compile PS file");
		return to_ret;
	}

	to_ret = d3d_device->CreatePixelShader(psblob->GetBufferPointer(), psblob->GetBufferSize(), NULL, &pixel_shader);
	psblob->Release();
	if (FAILED(to_ret))
		return to_ret;

	// Створення буферу вершин
	SimpleVertex vertices[3];
	vertices[0].pos.x = 0.0f;	vertices[0].pos.y = 0.5f;	vertices[0].pos.z = 0.5f;
	vertices[1].pos.x = 0.5f;	vertices[1].pos.y = -0.5f;  vertices[1].pos.z = 0.5f;
	vertices[2].pos.x = -0.5f;	vertices[2].pos.y = -0.5f;	vertices[2].pos.z = 0.5f;

	// структура, яка описує створюваний буфер
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(SimpleVertex) * 3; // розмір буфера = розмір 1 вершини * 3
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER; // тип буфера
	bd.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA init_data; // структура з даними буфера
	ZeroMemory(&init_data, sizeof(init_data)); 
	init_data.pSysMem = vertices; // вказує на створювані вершини

	// створюємо буфер
	to_ret = d3d_device->CreateBuffer(&bd, &init_data, &vertex_buffer);
	if (FAILED(to_ret))
		return to_ret;

	UINT stride = sizeof(SimpleVertex);
	UINT offset = 0;
	immediate_context->IASetVertexBuffers(0, 1, &vertex_buffer, &stride, &offset);

	// встановлення способу малювання вершин в буфері
	immediate_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	return to_ret;
}
inline void cleanup_device()
{
	if (immediate_context) 
		immediate_context->ClearState();
	if (vertex_buffer) 
		vertex_buffer->Release();
	if (vertex_layout)
		vertex_layout->Release();
	if (vertex_shader)
		vertex_shader->Release();
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
	float clear_color[4] = { 0.f, 0.f, 1.f, 1.f };
	immediate_context->ClearRenderTargetView(render_target_view, clear_color);

	immediate_context->VSSetShader(vertex_shader, NULL, 0);
	immediate_context->PSSetShader(pixel_shader, NULL, 0);
	immediate_context->Draw(3, 0);

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

	err = init_geometry();
	if (FAILED(err))
	{
		cleanup_device();
		grapgics_test_logger->critical("can't init geometry: {}", err);
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

