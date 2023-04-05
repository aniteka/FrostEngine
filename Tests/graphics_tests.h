#pragma once

#include <core/graphics/window.h>
#include <boost/test/unit_test.hpp>
#include <array>
#include <optional>
#include <core/graphics/graphic_engine_directx.h>

#include "core/graphics/shapes/shape_base.h"

BOOST_AUTO_TEST_SUITE(graphics_tests)

	BOOST_AUTO_TEST_CASE(MAIN)
{
	core::window main_window{
		{
			.width = 500,
			.height = 500,
			.title = CTEXT("OBAMA PRISM")
		} };

	core::graphic_engine_directx device{
		{
			.window = main_window
		} };
	device.set_clear_color({ 0.2f, 0.2f, 0.8f, 1.f });

	auto shape = std::make_shared<core::shape::shape_base>();
	shape->add_vertex({{ .0f, .5f, .5f }});
	shape->add_vertex({{ .5f, -.5f, .5f }});
	shape->add_vertex({{ -.5f, -.5f, .5f }});
	device.add_shape(shape);

	MSG msg;
	ZeroMemory(&msg, sizeof(msg));
	while(!main_window.should_close())
	{
		if(PeekMessage(&msg, main_window.get_native(), 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			device.render();
		}
	}

}

BOOST_AUTO_TEST_SUITE_END()

