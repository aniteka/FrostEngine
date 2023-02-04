#pragma once

#include <gtest/gtest.h>
#include <Core/Graphics/Window.h>
#include <Core/Exceptions.h>

#pragma comment (lib, "d3d12.lib")
#include <d3d12.h>
#include <Windows.h>
#include <dxgi1_6.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <string>
#include <wrl.h>
#include <shellapi.h>

#define TEST_NAME_GRAPHICS GraphicsTests

TEST(TEST_NAME_GRAPHICS, WindowTest)
{
	using namespace Core;
	auto mainWindow = Window({
		.width = 500,
		.height = 600,
		.title = TEXT("Test") });

	mainWindow.setXY(1, 2);
	EXPECT_EQ(mainWindow.getX(), 1);
	EXPECT_EQ(mainWindow.getY(), 2);

	mainWindow.move(100, 200);
	EXPECT_EQ(mainWindow.getX(), 101);
	EXPECT_EQ(mainWindow.getY(), 202);

	mainWindow.setX(10);
	mainWindow.setY(20);
	EXPECT_EQ(mainWindow.getX(), 10);
	EXPECT_EQ(mainWindow.getY(), 20);

	EXPECT_EQ(mainWindow.getWidth(), 500);
	EXPECT_EQ(mainWindow.getHeight(), 600);

	mainWindow.resize(200, 300);
	EXPECT_EQ(mainWindow.getWidth(), 200);
	EXPECT_EQ(mainWindow.getHeight(), 300);

	mainWindow.setWidth(1000);
	mainWindow.setHeight(1100);
	EXPECT_EQ(mainWindow.getWidth(), 1000);
	EXPECT_EQ(mainWindow.getHeight(), 1100);

	MSG msg = { };
	while (GetMessage(&msg, mainWindow.getNative(), 0, 0) > 0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

TEST(TEST_NAME_GRAPHICS, GraphicsTest)
{
	
}

