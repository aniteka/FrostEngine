#pragma once

#include <core/graphics/window.h>

#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(graphics_tests)

BOOST_AUTO_TEST_CASE(window_test)
{
	using namespace core;
	auto main_window = window({
		.width = 500,
		.height = 600,
		.title = CTEXT("Test") });

	main_window.set_xy(1, 2);
	BOOST_CHECK_EQUAL(main_window.get_x(), 1);
	BOOST_CHECK_EQUAL(main_window.get_y(), 2);

	main_window.move(100, 200);
	BOOST_CHECK_EQUAL(main_window.get_x(), 101);
	BOOST_CHECK_EQUAL(main_window.get_y(), 202);

	main_window.set_x(10);
	main_window.set_y(20);
	BOOST_CHECK_EQUAL(main_window.get_x(), 10);
	BOOST_CHECK_EQUAL(main_window.get_y(), 20);

	BOOST_CHECK_EQUAL(main_window.get_width(), 500);
	BOOST_CHECK_EQUAL(main_window.get_height(), 600);

	main_window.resize(200, 300);
	BOOST_CHECK_EQUAL(main_window.get_width(), 200);
	BOOST_CHECK_EQUAL(main_window.get_height(), 300);

	main_window.set_width(800);
	main_window.set_height(800);
	BOOST_CHECK_EQUAL(main_window.get_width(), 800);
	BOOST_CHECK_EQUAL(main_window.get_height(), 800);

	//MSG msg = { };
	//while (GetMessage(&msg, mainWindow.getNative(), 0, 0) > 0)
	//{
	//	TranslateMessage(&msg);
	//	DispatchMessage(&msg);
	//}
}

BOOST_AUTO_TEST_SUITE_END()

