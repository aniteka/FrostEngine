#include "Graphics.h"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <Core/Core.h>
#include <Core/Lib/Lib.h>
#include <iostream>

using namespace Core;

class HelloTriangleApplication
{
	using WindowPtr = std::unique_ptr < GLFWwindow, DeleteWith < glfwDestroyWindow > > ;
public:
	void run()
	{
		initWindow();
		initVulkan();
		mainLoop();
		cleanup();
	}
private:
	void initWindow()
	{
		glfwInit();
		// Turn off opengl api
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		// Turn off resizable
		glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
		// Create window
		window = WindowPtr(glfwCreateWindow(500, 500, "Vulkan", nullptr, nullptr));
	}
	void initVulkan()
	{
		createInstance();
	}
	void mainLoop()
	{
		// Check close flag
		while(!glfwWindowShouldClose(window.get()))
		{
			// Processes all pending events
			glfwPollEvents();
		}
	}
	void cleanup()
	{
		vkDestroyInstance(instance, nullptr);
		glfwTerminate();
	}
	void createInstance()
	{
		// Information about app(optional)
		VkApplicationInfo appInfo{};
		appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		appInfo.pApplicationName = "Hello Triangle";
		appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
		appInfo.pEngineName = "No Engine";
		appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
		appInfo.apiVersion = VK_API_VERSION_1_0;

		// Tell the lib which global extensions and validation layers we want to use
		VkInstanceCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		createInfo.pApplicationInfo = &appInfo;

		uint32_t glfwExtensionCount = 0;
		const char** glfwExtensions;

		// This function returns an array of names of Vulkan instance extensions required 
		// by GLFW for creating Vulkan surfaces for GLFW windows
		glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
		if (glfwExtensionCount == NULL)
			throw graphic_exception();

		createInfo.enabledExtensionCount = glfwExtensionCount;
		createInfo.ppEnabledExtensionNames = glfwExtensions;
		createInfo.enabledLayerCount = 0;

		if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS)
			throw graphic_exception();
	}

private:
	WindowPtr window;
	// Connecting between application and libVulkan. To init it
	// we need to get it information about out app
	VkInstance instance = nullptr;
#ifdef NDEBUG
	const bool USE_VALIDATION_LAYERS = false;
#else
	const bool USE_VALIDATION_LAYERS = true;
#endif
};

void Graphics::test()
{
	HelloTriangleApplication app;

	try {
		app.run();
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
		exit(EXIT_FAILURE);
	}
}

graphic_exception::graphic_exception(const char* msg, int error_code)
	: std::exception{ ("%s: %i"_f % msg % error_code).str().c_str() }
	, _error_code{error_code}
{}

graphic_exception::graphic_exception()
{
	const char* msg;
	_error_code = glfwGetError(&msg);
	std::exception::operator=(std::exception(("%s: %i"_f % msg % _error_code).str().c_str()));
}

int graphic_exception::get_code() const
{
	return _error_code;
}
