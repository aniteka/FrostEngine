#pragma once

#include "graphic_engine_base.h" 
#include "d3dcommon.h"
#include <core/text.h>
#include <core/memory.h>
#include <d3d11.h>

namespace core {class window; }
class ID3D11Device;
class ID3D11DeviceContext;
class IDXGISwapChain;
class ID3D11RenderTargetView;
class ID3D11VertexShader;

namespace core
{
	struct graphic_engine_directx_native
	{
		ID3D11Device* device;
		ID3D11DeviceContext* device_painter;
		IDXGISwapChain* swap_chain;
	};

	class graphic_engine_directx final
		: public graphic_engine_base
	{
	public:
		explicit graphic_engine_directx(const info_t& info);
		~graphic_engine_directx() override;

		const color_t& get_clear_color() const override;
		void set_clear_color(const color_t& color) override;

		[[nodiscard]] std::any get_native() override;

		void add_shape(shape_ptr_t shape) override;
		void remove_shape(shape_ptr_t shape) override;
		void remove_shapes_with_id(const core::id_t& shapes_id) override;
		shape_array_t get_shapes_by_id(const core::id_t& shapes_id) override;
		shape_array_t get_shapes_by_class(const std::type_info& class_type) override;
		std::size_t get_shapes_count() const override;

		void render() override;

	protected:
		const window& m_render_window;

		ID3D11Device* m_device = nullptr;
		ID3D11DeviceContext* m_device_painter = nullptr;
		IDXGISwapChain* m_swap_chain = nullptr;

		ID3D11RenderTargetView* m_posterior_buffer = nullptr;

		D3D_FEATURE_LEVEL m_future_level = D3D_FEATURE_LEVEL_11_1;
		color_t m_clear_color = { 1.f, 1.f, 1.f, 1.f };

		core::dynamic_array_t<core::sptr_t<core::shape::shape_base>> m_shape_array;

	private:
		void release_all();
		template <class T>
		void com_release(T*& to_release)
		{
			if (to_release)
			{
				to_release->Release();
				to_release = nullptr;
			}
		}

		void init_device(const info_t& info);
		void init_posterior_buffer(const info_t& info);
		void init_viewport(const info_t& info);

	private:
		//TODO dev, to del
		ID3D11VertexShader* m_dev_vertex_shader;
		ID3D11PixelShader* m_dev_pixel_shader;
		ID3D11InputLayout* m_dev_input_layout;
	};
}

