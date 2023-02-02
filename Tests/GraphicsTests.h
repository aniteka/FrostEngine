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
}

using namespace Microsoft::WRL;
_Use_decl_annotations_
void GetHardwareAdapter(IDXGIFactory1* pFactory,
						IDXGIAdapter1** ppAdapter,
						bool requestHighPerformanceAdapter)
{
	*ppAdapter = nullptr;
	ComPtr<IDXGIAdapter1> adapter;

	ComPtr<IDXGIFactory6> factory6;
	if(SUCCEEDED(pFactory->QueryInterface(IID_PPV_ARGS(&factory6))))
	{
		for (
			UINT adapterIndex = 0;
			SUCCEEDED(factory6->EnumAdapterByGpuPreference(
				adapterIndex,
				requestHighPerformanceAdapter ? DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE : DXGI_GPU_PREFERENCE_UNSPECIFIED,
				IID_PPV_ARGS(&adapter)));
			++adapterIndex)
		{
			DXGI_ADAPTER_DESC1 desc;
			adapter->GetDesc1(&desc);

			if (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE) 
				continue;
			
			if(SUCCEEDED(D3D12CreateDevice(adapter.Get(), D3D_FEATURE_LEVEL_11_0, _uuidof(ID3D12Device), nullptr)))
				break;
		}
	}

	if(adapter.Get() == nullptr)
	{
		for(UINT adapterIndex = 0; SUCCEEDED(pFactory->EnumAdapters1(adapterIndex, &adapter)); ++adapterIndex)
		{
			DXGI_ADAPTER_DESC1 desc;
			adapter->GetDesc1(&desc);

			if(desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE)
				continue;

			if(SUCCEEDED(D3D12CreateDevice(adapter.Get(), D3D_FEATURE_LEVEL_11_0, _uuidof(ID3D12Device), nullptr)))
				break;
		}
	}

	*ppAdapter = adapter.Detach();
}

TEST(TEST_NAME_GRAPHICS, GraphicsTest)
{
	
}

