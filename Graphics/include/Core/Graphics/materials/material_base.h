#pragma once
#include <core/text.h>
#include <core/utilities.h>

namespace core::materials
{
	class material_base
	{
	public:
		void set_fill_color(const core::color_t& fill_color);
		[[nodiscard]] core::color_t get_fill_color() const;

	private:
		core::color_t m_fill_color = {1.f, 1.f, 1.f, 1.f};
	};
}
