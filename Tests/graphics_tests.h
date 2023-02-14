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
	DirectX::XMFLOAT4 color; // кол≥р вершини
};

// —труктора константного буфера
struct ConstantBuffer
{
	DirectX::XMMATRIX world;		// матриц€ основного св≥ту
	DirectX::XMMATRIX view;			// матриц€ камери
	DirectX::XMMATRIX projection;	// матриц€ проекц≥њ
};

inline D3D_DRIVER_TYPE			driver_type = D3D_DRIVER_TYPE_NULL;
inline D3D_FEATURE_LEVEL		feature_level = D3D_FEATURE_LEVEL_11_0;
inline ID3D11Device*			d3d_device = NULL;
inline ID3D11DeviceContext*		immediate_context = NULL; 
inline IDXGISwapChain*			swap_chain = NULL;
inline ID3D11RenderTargetView*	render_target_view = NULL;
// Ѕуфер глибини(0 - дуже близько, 1 - безк≥нечно далеко)
// “екстура буфера глибини TODO
inline ID3D11Texture2D*			depth_stencil = NULL;
// Ѕуфер глибини
inline ID3D11DepthStencilView*	depth_stencil_view = NULL;

inline ID3D11VertexShader*		vertex_shader = NULL;
inline ID3D11PixelShader*		pixel_shader = NULL;
inline ID3D11InputLayout*		vertex_layout = NULL;
inline ID3D11Buffer*			vertex_buffer = NULL;
inline ID3D11Buffer*			index_buffer = NULL;
inline ID3D11Buffer*			constant_buffer = NULL;

DirectX::XMMATRIX world;
DirectX::XMMATRIX view;
DirectX::XMMATRIX projection;

inline HRESULT init_device(const core::window& window);
inline HRESULT init_geometry();
inline HRESULT init_matrixes(const core::window& window);

inline HRESULT compile_shader_from_file(const WCHAR* file_name, LPCSTR entry_point, LPCSTR shader_model, ID3DBlob** blob_out);
inline void set_matrixes(float angle);
inline void cleanup_device();
inline void render();

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

	err = init_matrixes(window);
	if( FAILED(err))
	{
		cleanup_device();
		grapgics_test_logger->critical("can't init matrixses: {}", err);
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

	// —творенн€ текстури дл€ заднього буферу
	D3D11_TEXTURE2D_DESC depth_desk;
	ZeroMemory(&depth_desk, sizeof(depth_desk));
	depth_desk.Width = width;
	depth_desk.Height = height;
	depth_desk.MipLevels = 1;	// р≥вень ≥нтерпол€ц≥њ
	depth_desk.ArraySize = 1;
	depth_desk.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depth_desk.SampleDesc.Count = 1;
	depth_desk.SampleDesc.Quality = 0;
	depth_desk.Usage = D3D11_USAGE_DEFAULT;
	depth_desk.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depth_desk.CPUAccessFlags = 0;
	depth_desk.MiscFlags = 0;

	to_ret = d3d_device->CreateTexture2D(
		&depth_desk,
		NULL,
		&depth_stencil);
	if (FAILED(to_ret))
		return to_ret;

	// —творенн€ виду глубини
	D3D11_DEPTH_STENCIL_VIEW_DESC dsv_desk;
	ZeroMemory(&dsv_desk, sizeof(dsv_desk));
	dsv_desk.Format = depth_desk.Format;
	dsv_desk.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	dsv_desk.Texture2D.MipSlice = 0;

	to_ret = d3d_device->CreateDepthStencilView(
		depth_stencil,
		&dsv_desk,
		&depth_stencil_view
	);
	if (FAILED(to_ret))
		return to_ret;

	// ¬становленн€ виду глибини
	immediate_context->OMSetRenderTargets(1, 
		&render_target_view, 
		depth_stencil_view);

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
inline HRESULT init_geometry()
{
	HRESULT to_ret = S_OK;

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

	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{	
			"POSITION",
			0, 
			DXGI_FORMAT_R32G32B32_FLOAT, 
			0,
			0,
			D3D11_INPUT_PER_VERTEX_DATA, 
			0
		},{ 
			"COLOR",
			0,
			DXGI_FORMAT_R32G32B32A32_FLOAT, 
			0,
			12, 
			D3D11_INPUT_PER_VERTEX_DATA,
			0
		}
	};

	UINT num_elements = ARRAYSIZE(layout);

	to_ret = d3d_device->CreateInputLayout(
		layout,
		num_elements,
		vsblob->GetBufferPointer(),
		vsblob->GetBufferSize(),
		&vertex_layout);
	vsblob->Release();

	if (FAILED(to_ret))
		return to_ret;

	immediate_context->IASetInputLayout(vertex_layout);

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

	using namespace DirectX;
	SimpleVertex vertices[] = 
		{	
			{XMFLOAT3{ 0.f, 1.5f, 0.f} , XMFLOAT4{1.f, 1.f, 0.f, 1.f}},
			{XMFLOAT3{-1.f, 0.f, -1.f} , XMFLOAT4{0.f, 1.f, 0.f, 1.f}},
			{XMFLOAT3{ 1.f, 0.f, -1.f} , XMFLOAT4{1.f, 0.f, 0.f, 1.f}},
			{XMFLOAT3{-1.f, 0.f,  1.f} , XMFLOAT4{0.f, 1.f, 1.f, 1.f}},
			{XMFLOAT3{ 1.f, 0.f,  1.f} , XMFLOAT4{1.f, 0.f, 1.f, 1.f}},
		};

	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(SimpleVertex) * 5;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER; 
	bd.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA init_data;
	ZeroMemory(&init_data, sizeof(init_data)); 
	init_data.pSysMem = vertices; 	

	to_ret = d3d_device->CreateBuffer(&bd, &init_data, &vertex_buffer);
	if (FAILED(to_ret))
		return to_ret;

	WORD indices[] =
	{
		0,2,1, 
		0,3,4, 
		0,1,3, 
		0,4,2, 

		1,2,3, 
		2,4,3, 
	};
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT; 
	bd.ByteWidth = sizeof(WORD) * 18;
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = 0;
	ZeroMemory(&init_data, sizeof(init_data)); 
	init_data.pSysMem = indices;
	to_ret = d3d_device->CreateBuffer(&bd, &init_data, &index_buffer);
	if (FAILED(to_ret))
		return to_ret;

	UINT stride = sizeof(SimpleVertex);
	UINT offset = 0;
	immediate_context->IASetVertexBuffers(0, 1, &vertex_buffer, &stride, &offset);

	immediate_context->IASetIndexBuffer(index_buffer, DXGI_FORMAT_R16_UINT, 0);

	immediate_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(ConstantBuffer);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;
	to_ret = d3d_device->CreateBuffer(&bd, NULL, &constant_buffer);
	if (FAILED(to_ret))
		return to_ret;

	return to_ret;
}
inline HRESULT init_matrixes(const core::window& window)
{
	using namespace DirectX;
	UINT width = window.get_width();
	UINT height = window.get_height();

	world = XMMatrixIdentity();

	auto eye		= XMVectorSet(0.f, 1.f, -8.f, 0.f);
	auto at		= XMVectorSet(0.f, 1.f, 0.f, 0.f);
	auto up		= XMVectorSet(0.f, 1.f, 0.f, 0.f);
	view = XMMatrixLookAtLH(eye, at, up);

	projection = XMMatrixPerspectiveFovLH(
		XM_PIDIV4,						
		width / (FLOAT)height,			
		0.01f,							
		100.f							
	);

	return S_OK;
}

inline void set_matrixes(float angle)
{
	using namespace DirectX;
	static float t = 0.f;
	if(driver_type == D3D_DRIVER_TYPE_REFERENCE)
	{
		t += (float)XM_PI * 0.0125f;
	}
	else
	{
		static DWORD time_start = 0;
		DWORD time_cur = GetTickCount();
		if (time_start == 0)
			time_start = time_cur;
		t = (float)(time_cur - time_start) / 1000.f;
	}

	// матриц€-орб≥та: позиц≥€ об'Їкта
	auto orbit = XMMatrixRotationY(-t + angle);
	// матриц€-ср≥н: повертаЇмо об'Їкт навколо своЇњ ос≥
	auto spin = XMMatrixRotationY(t * 2);
	// матриц€-позиц≥€: перем≥щенн€ на 3 одиниц≥ вл≥во в≥д початку координат
	auto translate = XMMatrixTranslation(-3.f, 0.f, 0.f);
	// матриц€-маштаб: скейлимо трикутник в половину рази
	auto scale = XMMatrixScaling(.5f, .5f, .5f);

	// трикутник знаходитьс€ в центр≥, кординати 0,0,0
	// —жимаЇмо -> повертаЇмо(ще не було перем≥щенн€) -> переносимо вл≥во ->
	// -> знову повертаЇмо(в≥дносно 0), так 6 трикутник≥в зможуть розташуватис€ на екран≥ р≥вном≥рно
	world = scale * spin * translate * orbit;

	ConstantBuffer cb;
	cb.world = XMMatrixTranspose(world);
	cb.view = XMMatrixTranspose(view);
	cb.projection = XMMatrixTranspose(projection);
	immediate_context->UpdateSubresource(constant_buffer, 
		0, NULL, 
		&cb, 
		0, 0);
}
inline void cleanup_device()
{
	if (immediate_context) 
		immediate_context->ClearState();
	if (constant_buffer)
		constant_buffer->Release();
	if (vertex_buffer) 
		vertex_buffer->Release();
	if (index_buffer)
		index_buffer->Release();
	if (vertex_layout)
		vertex_layout->Release();
	if (vertex_shader)
		vertex_shader->Release();
	if (depth_stencil)
		depth_stencil->Release();
	if (depth_stencil_view)
		depth_stencil_view->Release();
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

	// очищаЇмо буфер глибини до 1(максимум)
	immediate_context->ClearDepthStencilView(depth_stencil_view, D3D11_CLEAR_DEPTH, 1.f, 0);

	for (int i = 0; i < 6; ++i)
	{
		// тут малюЇмо кожен трикутник по в≥дд≥льност≥ на задньому буфер≥
		set_matrixes(i * (3.14f * 2.f) / 6.f);
		immediate_context->VSSetShader(vertex_shader, NULL, 0);
		immediate_context->VSSetConstantBuffers(0, 1, &constant_buffer);
		immediate_context->PSSetShader(pixel_shader, NULL, 0);
		immediate_context->DrawIndexed(18, 0, 0);
	}

	swap_chain->Present(0, 0);
}


BOOST_AUTO_TEST_SUITE_END()

