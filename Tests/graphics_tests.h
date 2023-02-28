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

#define SAFE_RELEASE(x) if((x)) { (x)->Release(); x = NULL; }

#define MX_SETWORLD 0x101


struct SimpleVertex
{
	DirectX::XMFLOAT3 pos;
	DirectX::XMFLOAT2 Tex;		// Координати текстури
	DirectX::XMFLOAT3 normal;	// нормаль вершини
};

struct ConstantBufferMatrixes
{
	DirectX::XMMATRIX world;			// матриця основного світу
	DirectX::XMMATRIX view;				// матриця камери
	DirectX::XMMATRIX projection;		// матриця проекції
};

struct ConstantBufferLight
{
	DirectX::XMFLOAT4 light_dir[2];		// напрямок світла
	DirectX::XMFLOAT4 light_color[2];	// колір джерела
	DirectX::XMFLOAT4 output_color;		// Колір квадрата по середині
};

inline D3D_DRIVER_TYPE			driver_type = D3D_DRIVER_TYPE_NULL;
inline D3D_FEATURE_LEVEL		feature_level = D3D_FEATURE_LEVEL_11_0;
inline ID3D11Device*			d3d_device = NULL;
inline ID3D11DeviceContext*		immediate_context = NULL; 
inline IDXGISwapChain*			swap_chain = NULL;
inline ID3D11RenderTargetView*	render_target_view = NULL;
inline ID3D11Texture2D*			depth_stencil = NULL;
inline ID3D11DepthStencilView*	depth_stencil_view = NULL;

inline ID3D11VertexShader*		vertex_shader = NULL;
inline ID3D11PixelShader*		pixel_shader = NULL;
inline ID3D11PixelShader*		pixel_shader_solid = NULL; // Піксельний шейдер для джерел світла
inline ID3D11InputLayout*		vertex_layout = NULL;
inline ID3D11Buffer*			vertex_buffer = NULL;
inline ID3D11Buffer*			index_buffer = NULL;
inline ID3D11Buffer*			CBMatrixes = NULL; // Константний буфер з інформацією про матриці 
inline ID3D11Buffer*			CBLight = NULL; // Константний буфер з інформацією про світло

inline DirectX::XMMATRIX	world;
inline DirectX::XMMATRIX	view;
inline DirectX::XMMATRIX	projection;
inline FLOAT				t = 0.f;		// Змінна часу

inline DirectX::XMFLOAT4 light_dirs[2];		// Напрямок світла (позиція джерел)
inline DirectX::XMFLOAT4 light_colors[2];	// Колір джерел

inline ID3D11ShaderResourceView*	texture_rv = NULL;		// Об'єкт текстури 
inline ID3D11SamplerState*			sampler_linear = NULL;	// Параметри накладання текстури

inline HRESULT init_device(const core::window& window);
inline HRESULT init_geometry();
inline HRESULT init_matrixes(const core::window& window);

inline void update_light();
inline void update_matrix(UINT light_index);	
inline void render();

inline void cleanup_device();
inline HRESULT compile_shader_from_file(const WCHAR* file_name, LPCSTR entry_point, LPCSTR shader_model, ID3DBlob** blob_out);

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
		grapgics_test_logger->error("{}", static_cast<const char*>(error_blob->GetBufferPointer()));
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

	// Створення текстури для заднього буферу
	D3D11_TEXTURE2D_DESC depth_desk;
	ZeroMemory(&depth_desk, sizeof(depth_desk));
	depth_desk.Width = width;
	depth_desk.Height = height;
	depth_desk.MipLevels = 1;	// рівень інтерполяції
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

	// Створення виду глубини
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

	// Встановлення виду глибини
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
	to_ret = compile_shader_from_file(L"shader.fx", "VertexShaderFunc", "vs_4_0", &vsblob);
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
			"TEXCOORD",
			0,
			DXGI_FORMAT_R32G32_FLOAT,
			0,
			12,
			D3D11_INPUT_PER_VERTEX_DATA,
			0
		}, {
			"NORMAL",
			0,
			DXGI_FORMAT_R32G32B32_FLOAT, 
			0,
			20, 
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
	to_ret = compile_shader_from_file(L"shader.fx", "PixelShaderFunc", "ps_4_0", &psblob);
	if(FAILED(to_ret))
	{
		grapgics_test_logger->critical("cannot compile PS file");
		return to_ret;
	}

	to_ret = d3d_device->CreatePixelShader(psblob->GetBufferPointer(), psblob->GetBufferSize(), NULL, &pixel_shader);
	psblob->Release();
	if (FAILED(to_ret))
		return to_ret;

	psblob = NULL;
	to_ret = compile_shader_from_file(L"shader.fx", "PixelShaderSolidFunc", "ps_4_0", &psblob);
	if(FAILED(to_ret))
	{
		grapgics_test_logger->critical("cannot compile PSSolid file");
		return to_ret;
	}

	to_ret = d3d_device->CreatePixelShader(psblob->GetBufferPointer(), psblob->GetBufferSize(), NULL, &pixel_shader_solid);
	psblob->Release();
	if (FAILED(to_ret))
		return to_ret;

	using namespace DirectX;
	SimpleVertex vertices[] = 
	{	/* координаты X, Y, Z            координаты текстры tu, tv   нормаль X, Y, Z        */
		{ XMFLOAT3(-1.0f, 1.0f, -1.0f),      XMFLOAT2(0.0f, 0.0f), XMFLOAT3(0.0f, 1.0f, 0.0f)},
		{ XMFLOAT3(1.0f, 1.0f, -1.0f), XMFLOAT2(1.0f, 0.0f), XMFLOAT3(0.0f, 1.0f, 0.0f)},
		{ XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT2(1.0f, 1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f)},
		{ XMFLOAT3(-1.0f, 1.0f, 1.0f), XMFLOAT2(0.0f, 1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f)},

		{ XMFLOAT3(-1.0f, -1.0f, -1.0f),     XMFLOAT2(0.0f, 0.0f), XMFLOAT3(0.0f, -1.0f, 0.0f)},
		{ XMFLOAT3(1.0f, -1.0f, -1.0f),      XMFLOAT2(1.0f, 0.0f), XMFLOAT3(0.0f, -1.0f, 0.0f)},
		{ XMFLOAT3(1.0f, -1.0f, 1.0f), XMFLOAT2(1.0f, 1.0f), XMFLOAT3(0.0f, -1.0f, 0.0f)},
		{ XMFLOAT3(-1.0f, -1.0f, 1.0f),      XMFLOAT2(0.0f, 1.0f), XMFLOAT3(0.0f, -1.0f, 0.0f)},

		{ XMFLOAT3(-1.0f, -1.0f, 1.0f),      XMFLOAT2(0.0f, 0.0f), XMFLOAT3(-1.0f, 0.0f, 0.0f)},
		{ XMFLOAT3(-1.0f, -1.0f, -1.0f),     XMFLOAT2(1.0f, 0.0f), XMFLOAT3(-1.0f, 0.0f, 0.0f)},
		{ XMFLOAT3(-1.0f, 1.0f, -1.0f),      XMFLOAT2(1.0f, 1.0f), XMFLOAT3(-1.0f, 0.0f, 0.0f)},
		{ XMFLOAT3(-1.0f, 1.0f, 1.0f), XMFLOAT2(0.0f, 1.0f), XMFLOAT3(-1.0f, 0.0f, 0.0f)},

		{ XMFLOAT3(1.0f, -1.0f, 1.0f), XMFLOAT2(0.0f, 0.0f), XMFLOAT3(1.0f, 0.0f, 0.0f)},
		{ XMFLOAT3(1.0f, -1.0f, -1.0f),      XMFLOAT2(1.0f, 0.0f), XMFLOAT3(1.0f, 0.0f, 0.0f)},
		{ XMFLOAT3(1.0f, 1.0f, -1.0f), XMFLOAT2(1.0f, 1.0f), XMFLOAT3(1.0f, 0.0f, 0.0f)},
		{ XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT2(0.0f, 1.0f), XMFLOAT3(1.0f, 0.0f, 0.0f)},

		{ XMFLOAT3(-1.0f, -1.0f, -1.0f),     XMFLOAT2(0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, -1.0f)},
		{ XMFLOAT3(1.0f, -1.0f, -1.0f),      XMFLOAT2(1.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, -1.0f)},
		{ XMFLOAT3(1.0f, 1.0f, -1.0f), XMFLOAT2(1.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, -1.0f)},
		{ XMFLOAT3(-1.0f, 1.0f, -1.0f),      XMFLOAT2(0.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, -1.0f)},

		{ XMFLOAT3(-1.0f, -1.0f, 1.0f),      XMFLOAT2(0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 1.0f)},
		{ XMFLOAT3(1.0f, -1.0f, 1.0f), XMFLOAT2(1.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 1.0f)},
		{ XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT2(1.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, 1.0f)},
		{ XMFLOAT3(-1.0f, 1.0f, 1.0f), XMFLOAT2(0.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, 1.0f)},
	};

	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(SimpleVertex) * 24;
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
		3,1,0,
		2,1,3,

		6,4,5,
		7,4,6,

		11,9,8,
		10,9,11,

		14,12,13,
		15,12,14,

		19,17,16,
		18,17,19,

		22,20,21,
		23,20,22
	};
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT; 
	bd.ByteWidth = sizeof(WORD) * 36;
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
	bd.ByteWidth = sizeof(ConstantBufferMatrixes);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;
	to_ret = d3d_device->CreateBuffer(&bd, NULL, &CBMatrixes);
	if (FAILED(to_ret))
		return to_ret;

	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(ConstantBufferLight);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;
	to_ret = d3d_device->CreateBuffer(&bd, NULL, &CBLight);
	if (FAILED(to_ret))
		return to_ret;

	// Підгружаємо текстуру
	to_ret = D3DX11CreateShaderResourceViewFromFile(
		d3d_device,
		L"testPhoto.dds", // TODO
		NULL,
		NULL,
		&texture_rv,
		NULL);
	if (FAILED(to_ret))
		return to_ret;

	D3D11_SAMPLER_DESC sampler_desc;
	ZeroMemory(&sampler_desc, sizeof(sampler_desc));
	sampler_desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;	// Тип фільтрації
	sampler_desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;		// Задаємо координати
	sampler_desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampler_desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampler_desc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sampler_desc.MinLOD = 0;
	sampler_desc.MaxLOD = D3D11_FLOAT32_MAX;
	to_ret = d3d_device->CreateSamplerState(&sampler_desc, &sampler_linear);
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

	auto eye		= XMVectorSet(0.f, 41.f, -10.f, 0.f);
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
inline void update_light()
{
	if(driver_type == D3D_DRIVER_TYPE_REFERENCE)
	{
		t += DirectX::XM_PI * 0.0125f;
	}
	else
	{
		static DWORD time_start = 0;
		DWORD time_cur = GetTickCount();
		if (time_start == 0)
			time_start = time_cur;
		t = (time_cur - time_start) / 1000.f;
	}

	// Встановлюємо початкові координати джерел світла.
	// потім за допомоною змінної t будемо шукати їх поточні координати
	light_dirs[0] = DirectX::XMFLOAT4(-0.577f, 0.577f, -0.577f, 1.0f);
	light_dirs[1] = DirectX::XMFLOAT4(0.0f, 0.0f, -1.0f, 1.0f);

	// Встановлюємо білий колір для джерела 1, та червоний для джерела 2
	light_colors[0] = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	light_colors[1] = DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);

	///////////////
	// За допомогою трансформацій повертаємо 2-ге джерело світла
	DirectX::XMMATRIX rotate = DirectX::XMMatrixRotationY(-2.f * t);
	DirectX::XMVECTOR light_dir = DirectX::XMLoadFloat4(&light_dirs[1]);
	light_dir = DirectX::XMVector3Transform(light_dir, rotate);
	DirectX::XMStoreFloat4(&light_dirs[1], light_dir);

	// За допомогою трансформацій повертаємо 1-ше джерело світла
	rotate = DirectX::XMMatrixRotationY(0.5f * t);
	light_dir = DirectX::XMLoadFloat4(&light_dirs[0]);
	light_dir = DirectX::XMVector3Transform(light_dir, rotate);
	DirectX::XMStoreFloat4(&light_dirs[0], light_dir);
	//////////////
	/// Послідовність дій: створюємо матрицю повороту ->
	/// -> завантаєужмо в змінну-вектор початкову позацію джерела світла ->
	///	-> трансфориуємо позицію відповідно до матриці ->
	///	-> функція XMStoreFloat4 копіює кординати із розширеного формату(вектора) в float4
}
inline void update_matrix(UINT light_index)
{
	using namespace DirectX;
	if (light_index == MX_SETWORLD)
	{
		world = DirectX::XMMatrixRotationAxis(DirectX::XMVectorSet(1.f, 1.f, 1.f, 0.f), t);
		light_index = 0;
	}
	else if (light_index < 2)
	{
		world = DirectX::XMMatrixTranslationFromVector(
			5.0f * DirectX::XMLoadFloat4(&light_dirs[light_index]));
		XMMATRIX light_scale = XMMatrixScaling(0.2f, 0.2f, 0.2f);
		world = light_scale * world;
	}
	else
		light_index = 0;

	ConstantBufferMatrixes cb1{
		.world = XMMatrixTranspose(world),
		.view = XMMatrixTranspose(view),
		.projection = XMMatrixTranspose(projection)
	};
	ConstantBufferLight cb2{
		.light_dir = { light_dirs[0], light_dirs[1]},
		.light_color = {light_colors[0], light_colors[1]},
		.output_color = light_colors[light_index]
	};
	immediate_context->UpdateSubresource(CBMatrixes, 0, NULL, &cb1, 0, 0);
	immediate_context->UpdateSubresource(CBLight, 0, NULL, &cb2, 0, 0);
}
inline void cleanup_device()
{
	if (immediate_context) 
		immediate_context->ClearState();
	SAFE_RELEASE(sampler_linear);
	SAFE_RELEASE(texture_rv);
	SAFE_RELEASE(CBMatrixes);
	SAFE_RELEASE(CBLight);
	SAFE_RELEASE(vertex_buffer);
	SAFE_RELEASE(index_buffer);
	SAFE_RELEASE(vertex_layout);
	SAFE_RELEASE(vertex_shader);
	SAFE_RELEASE(pixel_shader_solid);
	SAFE_RELEASE(pixel_shader);
	SAFE_RELEASE(depth_stencil);
	SAFE_RELEASE(depth_stencil_view);
	SAFE_RELEASE(render_target_view);
	SAFE_RELEASE(swap_chain);
	SAFE_RELEASE(immediate_context);
	SAFE_RELEASE(d3d_device);
}
inline void render()
{
	float clear_color[4] = { 0.f, 0.f, 1.f, 1.f };
	immediate_context->ClearRenderTargetView(render_target_view, clear_color);

	immediate_context->ClearDepthStencilView(depth_stencil_view, D3D11_CLEAR_DEPTH, 1.f, 0);

	update_light(); // встановка освітлення

	immediate_context->VSSetShader(vertex_shader, NULL, 0);
	immediate_context->VSSetConstantBuffers(0, 1, &CBMatrixes);
	immediate_context->VSSetConstantBuffers(1, 1, &CBLight);
	immediate_context->PSSetConstantBuffers(0, 1, &CBMatrixes);
	immediate_context->PSSetConstantBuffers(1, 1, &CBLight);
	immediate_context->PSSetShaderResources(0, 1, &texture_rv);
	immediate_context->PSSetSamplers(0, 1, &sampler_linear);

	immediate_context->PSSetShader(pixel_shader_solid, NULL, 0);
	for(int m = 0; m < 2; ++m)
	{
		update_matrix(m);
		immediate_context->DrawIndexed(36, 0, 0);
	}

	update_matrix(MX_SETWORLD);
	immediate_context->PSSetShader(pixel_shader, NULL, 0);
	immediate_context->DrawIndexed(36, 0, 0);

	swap_chain->Present(0, 0);
}


BOOST_AUTO_TEST_SUITE_END()

