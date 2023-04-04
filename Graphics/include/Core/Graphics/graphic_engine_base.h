#pragma once
#include <any>
#include <typeinfo>
#include <boost/noncopyable.hpp>
#include <core/text.h>
#include <core/memory.h>
#include <core/utilities.h>

namespace core
{
	namespace shape
	{
		class shape_base;
	}
	class window;
	struct graphics_info
	{
		const window& window;
		unsigned max_fps = 60;
	};

	class graphic_engine_base
		: boost::noncopyable
	{
	public:
		using info_t = graphics_info;
		using shape_ptr_t = core::sptr_t< core::shape::shape_base >;
		using shape_array_t = core::dynamic_array_t< shape_ptr_t >;

		graphic_engine_base() = default;
		virtual ~graphic_engine_base() = default;

		[[nodiscard]] virtual const color_t& get_clear_color() const = 0;
		virtual void set_clear_color(const color_t& color) = 0;

		[[nodiscard]] virtual std::any get_native() = 0;

		virtual void add_shape(shape_ptr_t shape) = 0;
		virtual void remove_shape(shape_ptr_t shape) = 0;
		virtual void remove_shapes_with_id(const core::id_t& shapes_id) = 0;
		virtual shape_array_t get_shapes_by_id(const core::id_t& shapes_id) = 0;
		virtual shape_array_t get_shape_by_class(const std::type_info& class_type) = 0;

		virtual void render() = 0;
	};
}

