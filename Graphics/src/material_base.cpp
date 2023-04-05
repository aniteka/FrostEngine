#include <core/graphics/materials/material_base.h>


void core::materials::material_base::set_fill_color(const core::color_t& fill_color)
{
	m_fill_color = fill_color;
}

core::color_t core::materials::material_base::get_fill_color() const
{
	return m_fill_color;
}
