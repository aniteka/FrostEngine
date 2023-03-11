#pragma once
#if !defined(WIN32)
#error DirectX can not by used without Windows
#endif

#include "graphic_engine_base.h" 
#include "d3dcommon.h"

namespace core {class window; }
class ID3D11Device;
class ID3D11DeviceContext;
class IDXGISwapChain;
class ID3D11RenderTargetView;

namespace core
{
	class graphic_engine_directx final
		: public graphic_engine_base
	{
	public:
		explicit graphic_engine_directx(const info_t& info);
		~graphic_engine_directx() override;
		graphic_engine_directx(graphic_engine_directx&&) noexcept = default;
		graphic_engine_directx& operator=(graphic_engine_directx&&) noexcept = default;

		void render() override;

		const color_t& get_clear_color() const override;
		void set_clear_color(const color_t& color) override;

	protected:
		const window& m_render_window;
		// create most of resources
		ID3D11Device* m_device = nullptr;
		// all about interaction with shaders, and painting
		ID3D11DeviceContext* m_device_painter = nullptr;
		// realize one or several painting surfaces
		// need for creating m_posterior_buffer
		IDXGISwapChain* m_swap_chain = nullptr;
		ID3D11RenderTargetView* m_posterior_buffer = nullptr;
		D3D_FEATURE_LEVEL m_future_level = D3D_FEATURE_LEVEL_11_1;

		color_t m_clear_color = { 1.f, 1.f, 1.f, 1.f };

		void release_all();
		template <class T>
		void com_release(T* to_release)
		{
			if (to_release)
				to_release->Release();
		}

	private:
		void init_device(const info_t& info);
		void init_posterior_buffer(const info_t& info);
		void init_viewport(const info_t& info);
		void compile_vertex_shader();
	};

	struct graphics_info
	{
		const window& window;
		unsigned max_fps = 60;
	};
}

