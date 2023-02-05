#pragma once

#include <boost/test/unit_test.hpp>
#include <Core/Graphics/Window.h>
#include <Core/Exceptions.h>

BOOST_AUTO_TEST_SUITE(GraphicsTests)

BOOST_AUTO_TEST_CASE(WindowTest)
{
	using namespace Core;
	auto mainWindow = Window({
		.width = 500,
		.height = 600,
		.title = TEXT("Test") });

	mainWindow.setXY(1, 2);
	BOOST_CHECK_EQUAL(mainWindow.getX(), 1);
	BOOST_CHECK_EQUAL(mainWindow.getY(), 2);

	mainWindow.move(100, 200);
	BOOST_CHECK_EQUAL(mainWindow.getX(), 101);
	BOOST_CHECK_EQUAL(mainWindow.getY(), 202);

	mainWindow.setX(10);
	mainWindow.setY(20);
	BOOST_CHECK_EQUAL(mainWindow.getX(), 10);
	BOOST_CHECK_EQUAL(mainWindow.getY(), 20);

	BOOST_CHECK_EQUAL(mainWindow.getWidth(), 500);
	BOOST_CHECK_EQUAL(mainWindow.getHeight(), 600);

	mainWindow.resize(200, 300);
	BOOST_CHECK_EQUAL(mainWindow.getWidth(), 200);
	BOOST_CHECK_EQUAL(mainWindow.getHeight(), 300);

	mainWindow.setWidth(800);
	mainWindow.setHeight(800);
	BOOST_CHECK_EQUAL(mainWindow.getWidth(), 800);
	BOOST_CHECK_EQUAL(mainWindow.getHeight(), 800);

	//MSG msg = { };
	//while (GetMessage(&msg, mainWindow.getNative(), 0, 0) > 0)
	//{
	//	TranslateMessage(&msg);
	//	DispatchMessage(&msg);
	//}
}

BOOST_AUTO_TEST_SUITE_END()

