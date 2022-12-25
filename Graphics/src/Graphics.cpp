#include "Graphics.h"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <Core/Core.h>
#include <Core/Lib/Lib.h>
#include <iostream>
#include <cstring>
#include <vector>
#include <map>
#include <optional>

using namespace Core;

struct QueueFamilyIndices {
	std::optional<uint32_t> graphicsFamily;
};

class HelloTriangleApplication
{
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
		window = glfwCreateWindow(500, 500, "Vulkan", nullptr, nullptr);
	}
	void initVulkan()
	{
		createInstance();
		setupDebugMessenger();
		pickPhysicalDevice();
		createLogicalDevice();
	}
	void mainLoop()
	{
		// Check close flag
		while(!glfwWindowShouldClose(window))
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
		vkDestroyDevice(device, nullptr);
		glfwDestroyWindow(window);
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
	void pickPhysicalDevice()
	{
		uint32_t deviceCount = 0;
		// Get count of GPUs
		vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);
		if (deviceCount == 0)
			throw graphic_exception("failde to find GPUs with Vulkan support", glfwGetError(NULL));
		std::vector<VkPhysicalDevice> devices(deviceCount);
		vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());
		
		// Pick the most suitable GPU
		std::multimap<int, VkPhysicalDevice> candidates;

		for (const auto& device : devices)
		{
			int score = rateDeviceSuitability(device);
			candidates.emplace(score, device);
		}

		if (candidates.rbegin()->first > 0)
			physicalDevice = candidates.rbegin()->second;
		else
			throw graphic_exception("failed to find a suitable GPU", 0);
	}
	int rateDeviceSuitability(VkPhysicalDevice device)
	{
		// Main properties 
		VkPhysicalDeviceProperties deviceProps;
		vkGetPhysicalDeviceProperties(device, &deviceProps);
		// Additional properties
		VkPhysicalDeviceFeatures deviceFeats;
		vkGetPhysicalDeviceFeatures(device, &deviceFeats);
		int score = 0;
		
		// Check if device is gpu
		if (deviceProps.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
			score += 1000;

		// Max possible texture size of textures affects graphics quality
		score += deviceProps.limits.maxImageDimension2D;

		if (!isDeviceSuitable(device))
			return 0;
		// App can't function without geometry shaders
		if (!deviceFeats.geometryShader)
			return 0;
		return score;
	}
	bool isDeviceSuitable(VkPhysicalDevice device)
	{
		return findQueueFamilies(device).graphicsFamily.has_value();
	}
	QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device)
	{
		QueueFamilyIndices to_ret;

		uint32_t queueFamilyCount = 0;
		vkGetPhysicalDeviceQueueFamilyProperties(
			device, 
			&queueFamilyCount, 
			nullptr);

		std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
		vkGetPhysicalDeviceQueueFamilyProperties(
			device, 
			&queueFamilyCount, 
			queueFamilies.data());

		int i = 0;
		for(const auto& queueFamily: queueFamilies)
		{
			if(queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
				to_ret.graphicsFamily = i;
			i++;
		}

		return to_ret;
	}

	void createLogicalDevice()
	{
		auto indices = findQueueFamilies(physicalDevice);

		VkDeviceQueueCreateInfo deviceQueueCreateInfo {};
		deviceQueueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		deviceQueueCreateInfo.queueFamilyIndex = indices.graphicsFamily.value_or(0);
		deviceQueueCreateInfo.queueCount = 1;
		float queuePriorities = 1.f;
		deviceQueueCreateInfo.pQueuePriorities = &queuePriorities;

		VkPhysicalDeviceFeatures physicalDeviceFeatures{};

		VkDeviceCreateInfo deviceCreateInfo {};
		deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
		deviceCreateInfo.pQueueCreateInfos = &deviceQueueCreateInfo;
		deviceCreateInfo.queueCreateInfoCount = 1;
		deviceCreateInfo.pEnabledFeatures = &physicalDeviceFeatures;
		deviceCreateInfo.enabledExtensionCount = 0;

		if (USE_VALIDATION_LAYERS) {
			deviceCreateInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
			deviceCreateInfo.ppEnabledLayerNames = validationLayers.data();
		}
		else {
			deviceCreateInfo.enabledLayerCount = 0;
		}

		if(vkCreateDevice(physicalDevice, &deviceCreateInfo, nullptr, &device) != VK_SUCCESS )
		{
			throw graphic_exception("failed to create logical device", -1);
		}

		vkGetDeviceQueue(device, indices.graphicsFamily.value_or(0), 0, &graphicsQueue);
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
	GLFWwindow* window;
	// Connecting between application and libVulkan. To init it
	// we need to get it information about out app
	VkInstance instance = nullptr;

	VkDebugUtilsMessengerEXT debugMessenger;

	VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
	VkDevice device;
	VkQueue graphicsQueue;

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
	: std::runtime_error{ (""_f % 1).str().data() }
	, _error_code{error_code}
{}

graphic_exception::graphic_exception()
    : std::runtime_error{""}
{
	const char* msg;
	_error_code = glfwGetError(&msg);
	std::runtime_error::operator=(std::runtime_error(("%s: %i"_f % msg % _error_code).str().c_str()));
}

int graphic_exception::get_code() const
{
	return _error_code;
}
