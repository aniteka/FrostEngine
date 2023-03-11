#include <graphic_engine_directx.h>

#include <Window.h>
#include <core/exceptions.h>
#include <d3d11.h>
#include <D3DX11.h>
#include <d3dcompiler.h>

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

core::graphic_engine_directx::graphic_engine_directx(const info_t& info)
	: m_render_window{ info.window }
{
	init_device(info);
	init_posterior_buffer(info);
	init_viewport(info);
	compile_vertex_shader();
}

core::graphic_engine_directx::~graphic_engine_directx()
{
	release_all();
}

void core::graphic_engine_directx::render()
{
	m_device_painter->ClearRenderTargetView(m_posterior_buffer, m_clear_color.data());

	m_swap_chain->Present(1, 0);
}

const core::color_t& core::graphic_engine_directx::get_clear_color() const
{
	return m_clear_color;
}

void core::graphic_engine_directx::set_clear_color(const color_t& color)
{
	m_clear_color = color;
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

void core::graphic_engine_directx::compile_vertex_shader()
{
	const char simple_vertex_shader[] = R"(
	float4 VS(float4 Pos : POSITION) : SV_POSITION
	{
		return Pos;
	})";
	try
	{
	auto vertex_shader_blob = compile_shader_from_memory_runtime(
		simple_vertex_shader, sizeof(simple_vertex_shader) / sizeof(char),
		"VS", "vs_4_0");
	}
	catch (const std::exception& e)
	{
		printf("%s", e.what());
		std::exit(-1);
	}
}

