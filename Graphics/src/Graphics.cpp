#include "Graphics.h"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <Core/Core.h>
#include <Core/Lib/Lib.h>
#include <iostream>
#include <cstring>
#include <vector>

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
		setupDebugMessenger();
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
		if(USE_VALIDATION_LAYERS)
			DestroyDebugUtilsMessengerEXT(instance, debugMessenger, nullptr);
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

		auto extensions = getRequiredExtensions();
		createInfo.enabledExtensionCount = (uint32_t)extensions.size();
		createInfo.ppEnabledExtensionNames = extensions.data();

		if (USE_VALIDATION_LAYERS)
		{
			if (!checkValidationLayerSupport())
				throw graphic_exception{ "validation layers requested, but not available!", -1 };
			createInfo.enabledLayerCount = (uint32_t)validationLayers.size();
			createInfo.ppEnabledLayerNames = validationLayers.data();
		}
		else
			createInfo.enabledLayerCount = 0;

		if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS)
			throw graphic_exception();
	}
	bool checkValidationLayerSupport()
	{
		uint32_t layersCount;
		// Getting list of available layers
		vkEnumerateInstanceLayerProperties(&layersCount, nullptr);

		std::vector<VkLayerProperties> availableLayers(layersCount);
		vkEnumerateInstanceLayerProperties(&layersCount, availableLayers.data());

		for (const auto& i : validationLayers)
		{
			for (const auto& j : availableLayers)
				if (strcmp(j.layerName, i) == 0)
					goto NEXT;
			return false;
			NEXT:{}
		}

		return true;
	}
	std::vector<const char*> getRequiredExtensions()
	{
		uint32_t glfwExtensionCount = 0;
		const char** glfwExtensions;
		// This function returns an array of names of Vulkan instance extensions required 
		// by GLFW for creating Vulkan surfaces for GLFW windows
		glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
		if (glfwExtensionCount == NULL)
			throw graphic_exception();

		std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);
		// Need to use callback error handling functions
		if (USE_VALIDATION_LAYERS)
		{
			extensions.emplace_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
		}
		return extensions;
	}
	void setupDebugMessenger()
	{
		if (!USE_VALIDATION_LAYERS) return;
		VkDebugUtilsMessengerCreateInfoEXT createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
		// the severity levels for which the callback function will be called. 
		createInfo.messageSeverity
			= VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT
			| VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT
			| VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT
			| VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT;
		// allows you to filter messages by type
		createInfo.messageType
			= VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT
			| VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT
			| VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
		createInfo.pfnUserCallback = debugCallback;
		createInfo.pUserData = nullptr;

		auto res = CreateDebugUtilsMessangerEXT(instance, &createInfo, nullptr, &debugMessenger);
		if (res != VK_SUCCESS)
			throw graphic_exception("failed to setup debug messenger", glfwGetError(NULL));
	}

	static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
		// The first parameter specifies the severity of the message.
		VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
		// See doc
		VkDebugUtilsMessageTypeFlagsEXT messageType,
		// Message
		const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
		void* pUserData) {

		std::cerr << "validation layer: " << pCallbackData->pMessage << std::endl;

		// If not false, abort the call
		return VK_FALSE;
	}

	VkResult CreateDebugUtilsMessangerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger)
	{
		auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
		if (func == nullptr)
			return VK_ERROR_EXTENSION_NOT_PRESENT;
		return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
	}
	void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator) {
		auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
		if (func != nullptr) {
			func(instance, debugMessenger, pAllocator);
		}
	}

private:
	WindowPtr window;
	// Connecting between application and libVulkan. To init it
	// we need to get it information about out app
	VkInstance instance = nullptr;

	VkDebugUtilsMessengerEXT debugMessenger;

	// All useful validations collected in VK_LAYER_KHRONOS_validation
	const std::vector<const char*> validationLayers{
		"VK_LAYER_KHRONOS_validation"
	};

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
