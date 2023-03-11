#pragma once
#include <boost/noncopyable.hpp>
#include <array>

namespace core
{
	using color_t = std::array<float, 4>;
	struct graphics_info;

	class graphic_engine_base
		: boost::noncopyable
	{
	public:
		using info_t = graphics_info;

		graphic_engine_base() = default;
		graphic_engine_base(graphic_engine_base&&) noexcept = default;
		graphic_engine_base& operator=(graphic_engine_base&&) noexcept = default;
		virtual ~graphic_engine_base() = default;

		virtual void render() = 0;

		virtual const color_t& get_clear_color() const = 0;
		virtual void set_clear_color(const color_t& color) = 0;
	};
}

