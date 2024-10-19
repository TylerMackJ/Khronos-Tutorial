#include "Instance.hpp"

#include <iostream>

#include "DebugMessenger.hpp"
#include "HelloTriangleApplication.hpp"

using App = HelloTriangleApplication;

Instance::Instance()
{
	// Check for validation layer support
	if( App::get().enableValidationLayers && !checkValidationLayerSupport() )
	{
		throw std::runtime_error( "validation layers requested, but not available!" );
	}

	// Setup app info
	VkApplicationInfo appInfo{};
	appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	appInfo.pApplicationName = "Hello Triangle";
	appInfo.applicationVersion = VK_MAKE_VERSION( 1, 0, 0 );
	appInfo.pEngineName = "No Engine";
	appInfo.engineVersion = VK_MAKE_VERSION( 1, 0, 0 );
	appInfo.apiVersion = VK_API_VERSION_1_0;

	// Setup create info
	VkInstanceCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	createInfo.pApplicationInfo = &appInfo;

	auto extensions = getRequiredExtensions();
	createInfo.enabledExtensionCount = static_cast< uint32_t >(extensions.size());
	createInfo.ppEnabledExtensionNames = extensions.data();

	VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};
	if( App::get().enableValidationLayers )
	{
		createInfo.enabledLayerCount = static_cast< uint32_t >(App::get().validationLayers.size());
		createInfo.ppEnabledLayerNames = App::get().validationLayers.data();

		DebugMessenger::populateDebugMessengerCreateInfo( debugCreateInfo );
		createInfo.pNext = ( VkDebugUtilsMessengerCreateInfoEXT* )&debugCreateInfo;
	}
	else
	{
		createInfo.enabledLayerCount = 0;
		createInfo.pNext = nullptr;
	}

	// Create Vulkan instance
	if( vkCreateInstance( &createInfo, nullptr, &instance ) != VK_SUCCESS )
	{
		throw std::runtime_error( "failed to create instance!" );
	}

	#ifndef NDEBUG
		printExtensions();
	#endif
}

Instance::~Instance()
{
	vkDestroyInstance( instance, nullptr );
}

bool Instance::checkValidationLayerSupport()
{
	uint32_t layerCount;
	vkEnumerateInstanceLayerProperties( &layerCount, nullptr );

	std::vector<VkLayerProperties> availableLayers( layerCount );
	vkEnumerateInstanceLayerProperties( &layerCount, availableLayers.data() );

	for( const char* layerName : App::get().validationLayers )
	{
		bool layerFound = false;

		for( const auto& layerProperties : availableLayers )
		{
			if( strcmp( layerName, layerProperties.layerName ) == 0 )
			{
				layerFound = true;
				break;
			}
		}

		if( !layerFound )
		{
			return false;
		}
	}

	return true;
}

std::vector<const char*> Instance::getRequiredExtensions()
{
	uint32_t glfwExtensionCount = 0;
	const char** glfwExtensions = glfwGetRequiredInstanceExtensions( &glfwExtensionCount );

	std::vector<const char*> extensions( glfwExtensions, glfwExtensions + glfwExtensionCount );

	if( App::get().enableValidationLayers )
	{
		extensions.push_back( VK_EXT_DEBUG_UTILS_EXTENSION_NAME );
	}

	return extensions;
}

void Instance::printExtensions()
{
	uint32_t extensionCount = 0;
	vkEnumerateInstanceExtensionProperties( nullptr, &extensionCount, nullptr );
	std::vector<VkExtensionProperties> extensions( extensionCount );
	vkEnumerateInstanceExtensionProperties( nullptr, &extensionCount, extensions.data() );

	std::cout << "available extensions:" << std::endl;
	for( const auto& extension : extensions )
	{
		std::cout << "\t" << extension.extensionName << std::endl;
	}
}