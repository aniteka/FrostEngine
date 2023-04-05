#include <core/graphics/shapes/shape_base.h>


core::shape::shape_base::shape_base() = default;
core::shape::shape_base::~shape_base() = default;
core::shape::shape_base::shape_base(const shape_base& other) = default;
core::shape::shape_base& core::shape::shape_base::operator=(const shape_base& other) = default;
core::shape::shape_base::shape_base(shape_base&& other) noexcept = default;
core::shape::shape_base& core::shape::shape_base::operator=(shape_base&& other) noexcept = default;


const core::id_t& core::shape::shape_base::get_id() const
{
	return m_id;
}

void core::shape::shape_base::set_id(const core::id_t& new_id)
{
	m_id = new_id;
}

void core::shape::shape_base::set_scale(const core::vector3f& scale)
{
	m_scale = scale;
}

const core::vector3f& core::shape::shape_base::get_scale() const
{
	return m_scale;
}

void core::shape::shape_base::set_location(const core::vector3f& location)
{
	m_location = location;
}

const core::vector3f& core::shape::shape_base::get_location()
{
	return m_location;
}

void core::shape::shape_base::add_vertex(const core::shape::vertex& vertex)
{
	m_vertex_array.push_back(vertex);
}

void core::shape::shape_base::set_material(core::sptr_t<core::materials::material_base> material)
{
	m_material.swap(material);
}

core::sptr_t<core::materials::material_base> core::shape::shape_base::get_material() const
{
	return m_material;
}

