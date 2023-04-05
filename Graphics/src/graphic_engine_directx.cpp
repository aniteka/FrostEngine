#include <graphic_engine_directx.h>

#include <Window.h>
#include <core/exceptions.h>
#include <d3d11.h>
#include <D3DX11.h>
#include <d3dcompiler.h>
#include <functional>
#include <ranges>
#include "shapes/shape_base.h"

ID3DBlob* compile_shader_from_file_runtime( const WCHAR* file_name, LPCSTR entry_point, LPCSTR shader_model )
{
	DWORD shader_flags = D3DCOMPILE_ENABLE_STRICTNESS;
	ID3DBlob* error_blob;
	ID3DBlob* to_ret;
	const auto err = D3DX11CompileFromFile(
		file_name,
		nullptr,
		nullptr,
		entry_point,
		shader_model,
		shader_flags,
		0,
		nullptr,
		&to_ret,
		&error_blob,
		nullptr);

	if(FAILED(err))
	{
		std::string err_msg = "Failed to compile shader";
		if (error_blob)
		{
			err_msg = static_cast<const char*>(error_blob->GetBufferPointer());
			error_blob->Release();
		}
		if (to_ret)
			to_ret->Release();
		throw core::exceptions::graphics_exception(err_msg, err);
	}

	return to_ret;
}

ID3DBlob* compile_shader_from_memory_runtime(LPCSTR code, size_t code_len, LPCSTR entry_point, LPCSTR shader_model)
{
	DWORD shader_flags = D3DCOMPILE_ENABLE_STRICTNESS;
	ID3DBlob* error_blob;
	ID3DBlob* to_ret;
	const auto err = D3DX11CompileFromMemory(
		code,
		code_len,
		nullptr,
		nullptr,
		nullptr,
		entry_point,
		shader_model,
		shader_flags,
		0,
		nullptr,
		&to_ret,
		&error_blob,
		nullptr);

	if(FAILED(err))
	{
		std::string err_msg = "Failed to compile shader";
		if (error_blob)
		{
			err_msg = static_cast<const char*>(error_blob->GetBufferPointer());
			error_blob->Release();
		}
		if (to_ret)
			to_ret->Release();
		throw core::exceptions::graphics_exception(err_msg, err);
	}

	return to_ret;
}

/////////////////////////////////////////////////////////////////
//////////////////////////////CLASS//////////////////////////////
/////////////////////////////////////////////////////////////////

core::graphic_engine_directx::graphic_engine_directx(const info_t& info)
	: m_render_window{ info.window }
{
	init_device(info);
	init_posterior_buffer(info);
	init_viewport(info);

	HRESULT err = S_OK;

	char code[] = R"(
	float4 VS(float4 Pos : POSITION, float2 Texture : TEXCOORD0, float3 norm : NORMAL) : SV_POSITION
	{
		return Pos;
	}

	float4 PS(float4 Pos : SV_POSITION) : SV_Target
	{
		return float4(1.f, 1.f, 1.f, 1.f);
	}
	)";

	auto vs_blob =
		compile_shader_from_memory_runtime(code, std::size(code), "VS", "vs_4_1");
	err = m_device->CreateVertexShader(
		vs_blob->GetBufferPointer(),
		vs_blob->GetBufferSize(),
		nullptr,
		&m_dev_vertex_shader
	);
	if(FAILED(err))
		throw exceptions::graphics_exception("Error while creating vertex shader", err);

	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{"POSITION",
		0,
		DXGI_FORMAT_R32G32B32_FLOAT,
		0,0,
		D3D11_INPUT_PER_VERTEX_DATA,
		0},
		{"TEXCOORD",
			0,
			DXGI_FORMAT_R32G32_FLOAT,
			0, 12,
			D3D11_INPUT_PER_VERTEX_DATA,
			0},
		{"NORMAL",
		0,
		DXGI_FORMAT_R32G32B32_FLOAT,
		0,12 + 8,
		D3D11_INPUT_PER_VERTEX_DATA,
		0},
	};
	auto layout_size = ARRAYSIZE(layout);

	err = m_device->CreateInputLayout(layout, layout_size, 
		vs_blob->GetBufferPointer(), 
		vs_blob->GetBufferSize(), 
		&m_dev_input_layout);
	vs_blob->Release();
	if(FAILED(err))
		throw exceptions::graphics_exception("Error while creating input layout", err);
	m_device_painter->IASetInputLayout(m_dev_input_layout);

	auto ps_blob
		= compile_shader_from_memory_runtime(code, std::size(code), "PS", "ps_4_1");
	err = m_device->CreatePixelShader(
		ps_blob->GetBufferPointer(),
		ps_blob->GetBufferSize(),
		nullptr,
		&m_dev_pixel_shader
	);
	ps_blob->Release();
	if(FAILED(err))
		throw exceptions::graphics_exception("Error while creating pixel shader", err);
}

core::graphic_engine_directx::~graphic_engine_directx()
{
	release_all();
}

void core::graphic_engine_directx::render()
{
	m_device_painter->ClearRenderTargetView(m_posterior_buffer, m_clear_color.data());

	for (const auto& shape : m_shape_array)
	{
		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(decltype(shape->m_vertex_array)::value_type) * shape->m_vertex_array.size();
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = 0;

		D3D11_SUBRESOURCE_DATA srd;
		ZeroMemory(&srd, sizeof(srd));
		srd.pSysMem = shape->m_vertex_array.data();

		ID3D11Buffer* buff;
		unsigned stride = sizeof(core::shape::vertex);
		unsigned offset = 0;
		auto err = m_device->CreateBuffer(&bd, &srd, &buff);
		if (FAILED(err))
			throw exceptions::graphics_exception("Create Buffer error", err);
		m_device_painter->IASetVertexBuffers(0, 1, &buff, &stride, &offset);
		m_device_painter->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		m_device_painter->VSSetShader(m_dev_vertex_shader, nullptr, 0);
		m_device_painter->PSSetShader(m_dev_pixel_shader, nullptr, 0);
		m_device_painter->Draw(shape->m_vertex_array.size(), 0 );
		buff->Release();
	}

	m_swap_chain->Present(0, 0);
}

const core::color_t& core::graphic_engine_directx::get_clear_color() const
{
	return m_clear_color;
}

void core::graphic_engine_directx::set_clear_color(const color_t& color)
{
	m_clear_color = color;
}

std::any core::graphic_engine_directx::get_native()
{
	return core::graphic_engine_directx_native{
		.device = m_device,
		.device_painter = m_device_painter,
		.swap_chain = m_swap_chain,
	};
}

void core::graphic_engine_directx::add_shape(core::graphic_engine_base::shape_ptr_t shape)
{
	m_shape_array.emplace_back(shape);
}

void core::graphic_engine_directx::remove_shape(core::graphic_engine_base::shape_ptr_t shape)
{
	const auto to_erase =
	std::ranges::find_if(
		m_shape_array, 
		[&shape](auto t) { return t.get() == shape.get(); });

	m_shape_array.erase(to_erase);
}

void core::graphic_engine_directx::remove_shapes_with_id(const core::id_t& shapes_id)
{
	auto [beg, end] = std::ranges::remove_if(m_shape_array, [](const shape_ptr_t& arg) { return false; });
	m_shape_array.erase(beg, end);
}

core::graphic_engine_base::shape_array_t core::graphic_engine_directx::get_shapes_by_id(
	const core::id_t& shapes_id)
{
	return m_shape_array
		| std::views::filter([&shapes_id](const shape_ptr_t& arg) { return arg->get_id() == shapes_id; })
		| std::ranges::to<shape_array_t>();
}

core::graphic_engine_base::shape_array_t core::graphic_engine_directx::get_shape_by_class(const std::type_info& class_type)
{
	return m_shape_array
		| std::views::filter([&class_type](const shape_ptr_t& arg)
			{return typeid(*arg) == class_type; })
		| std::ranges::to<shape_array_t>();
}

void core::graphic_engine_directx::release_all()
{
	if (m_device_painter)
		m_device_painter->ClearState();
	com_release(m_posterior_buffer);
	com_release(m_swap_chain);
	com_release(m_device_painter);
	com_release(m_device);
}

/////////////////////////////////////////////////////////////////
////////////////////////////PRIVATE//////////////////////////////
/////////////////////////////////////////////////////////////////

void core::graphic_engine_directx::init_device(const info_t& info)
{
	D3D_FEATURE_LEVEL feature_level[] =
	{
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0
	};

	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.BufferCount = 1;
	sd.BufferDesc.Width = m_render_window.get_width();
	sd.BufferDesc.Height = m_render_window.get_height();
	sd.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = info.max_fps;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow = m_render_window.get_native();
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.Windowed = true;

	const auto err = D3D11CreateDeviceAndSwapChain(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		0,
		feature_level,
		sizeof(feature_level) / sizeof(D3D_FEATURE_LEVEL),
		D3D11_SDK_VERSION,
		&sd,
		&m_swap_chain,
		&m_device,
		&m_future_level,
		&m_device_painter
	);
	if(FAILED(err))
	{
		release_all();
		throw exceptions::graphics_exception("Failed to create devices", err);
	}
}

void core::graphic_engine_directx::init_posterior_buffer(const info_t& info)
{
	ID3D11Texture2D* back_buffer_texture = nullptr;
	auto err = m_swap_chain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&back_buffer_texture);
	if(FAILED(err))
	{
		release_all();
		throw exceptions::graphics_exception("Failed to get buffer from swap chain", err);
	}

	err = 
		m_device->CreateRenderTargetView(back_buffer_texture, nullptr, &m_posterior_buffer);
	if(FAILED(err))
	{
		release_all();
		throw exceptions::graphics_exception("Failed to create render target view", err);
	}

	m_device_painter->OMSetRenderTargets(1, &m_posterior_buffer, nullptr);
}

void core::graphic_engine_directx::init_viewport(const info_t& info)
{
	D3D11_VIEWPORT vp;
	vp.Width = m_render_window.get_width();
	vp.Height = m_render_window.get_height();
	vp.MinDepth = 0.f;
	vp.MaxDepth = 1.f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;

	m_device_painter->RSSetViewports(1, &vp);
}

