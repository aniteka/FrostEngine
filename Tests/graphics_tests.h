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

	core::sptr_t<core::shape::shape_base> shape_ptr;
	for (int i = 0; i < 5; ++i)
	{
		auto pos_a = core::vector3f{ .0f, .5f, .5f } / 5;	
		auto pos_b = core::vector3f{ .5f, -.5f, .5f } / 5;	
		auto pos_c = core::vector3f{ -.5f, -.5f, .5f } / 5;	
		pos_a.x += 0.1f * (float)i;
		pos_b.x += 0.1f * (float)i;
		pos_c.x += 0.1f * (float)i;
		auto shape = std::make_shared<core::shape::shape_base>();
		shape->add_vertex({ pos_a });
		shape->add_vertex({ pos_b });
		shape->add_vertex({ pos_c });
		shape->set_id(CFORMAT("vertex{}", i));
		device.add_shape(shape);
		shape_ptr = shape;
	}
	device.remove_shape(shape_ptr);
	BOOST_REQUIRE_EQUAL(device.get_shapes_count(), 4);
	device.remove_shapes_with_id(CTEXT("vertex4"));
	BOOST_REQUIRE_EQUAL(device.get_shapes_count(), 4);
	device.remove_shapes_with_id(CTEXT("vertex0"));
	BOOST_REQUIRE_EQUAL(device.get_shapes_count(), 3);
	BOOST_REQUIRE_EQUAL(
		device.get_shapes_by_class(
			typeid(
				*device.get_shapes_by_id(
					CTEXT("vertex1")
				).at(0))
		).size(),
		3);

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

