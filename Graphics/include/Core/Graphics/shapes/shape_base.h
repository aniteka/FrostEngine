#pragma once
#include <any>
#include <core/utilities.h>
#include <core/memory.h>
#include <core/text.h>
#include <core/graphics/materials/material_base.h>

namespace core::shape
{
	struct vertex
	{
		core::vector3f position;
		core::vector2f texture_coord;
		core::vector3f normal;
	};

	class shape_base
	{
	public:
		shape_base();
		virtual ~shape_base();
		shape_base(const shape_base& other);
		shape_base& operator=(const shape_base& other);
		shape_base(shape_base&& other) noexcept;
		shape_base& operator=(shape_base&& other) noexcept;

		[[nodiscard]] const core::id_t& get_id() const;
		void set_id(const core::id_t& new_id);

		void set_scale(const core::vector3f& scale);
		[[nodiscard]] const core::vector3f& get_scale() const;

		void set_location(const core::vector3f& location);
		const core::vector3f& get_location();

		void add_vertex(const core::shape::vertex& vertex);

		void set_material(core::sptr_t<core::materials::material_base> material);
		core::sptr_t<core::materials::material_base> get_material() const;

		friend class graphic_engine_base;
		friend class graphic_engine_directx;
	protected:
		core::id_t m_id = CTEXT("None");
		core::vector3f m_scale = { 1.f,1.f,1.f };
		core::vector3f m_location = { 0.f, 0.f, 0.f };
		core::dynamic_array_t<core::shape::vertex> m_vertex_array;
		core::sptr_t<core::materials::material_base> m_material;

		std::any m_engine_data;
	};
}

