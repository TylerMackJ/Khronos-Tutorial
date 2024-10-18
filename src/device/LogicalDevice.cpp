#include "LogicalDevice.hpp"

#include <set>

#include "HelloTriangleApplication.hpp"

LogicalDevice::LogicalDevice()
{
	PhysicalDevice::QueueFamilyIndices indices = HelloTriangleApplication::getPhysicalDevice().getQueueFamilyIndices();

	std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
	std::set<uint32_t> uniqueQueueFamilies = { indices.graphicsFamily.value(), indices.presentFamily.value() };

	float queuePriority = 1.0f;
	for( uint32_t queueFamily : uniqueQueueFamilies )
	{
		VkDeviceQueueCreateInfo queueCreateInfo{};
		queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		queueCreateInfo.queueFamilyIndex = queueFamily;
		queueCreateInfo.queueCount = 1;
		queueCreateInfo.pQueuePriorities = &queuePriority;
		queueCreateInfos.push_back( queueCreateInfo );
	}

	VkPhysicalDeviceFeatures deviceFeatures{};

	VkDeviceCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	createInfo.pQueueCreateInfos = queueCreateInfos.data();
	createInfo.queueCreateInfoCount = static_cast< uint32_t >(queueCreateInfos.size());
	createInfo.pEnabledFeatures = &deviceFeatures;
	createInfo.enabledExtensionCount = static_cast< uint32_t >(HelloTriangleApplication::deviceExtensions.size());
	createInfo.ppEnabledExtensionNames = HelloTriangleApplication::deviceExtensions.data();
	if( HelloTriangleApplication::enableValidationLayers )
	{
		createInfo.enabledLayerCount = static_cast< uint32_t >(HelloTriangleApplication::validationLayers.size());
		createInfo.ppEnabledLayerNames = HelloTriangleApplication::validationLayers.data();
	}
	else
	{
		createInfo.enabledLayerCount = 0;
	}

	if( vkCreateDevice( HelloTriangleApplication::getPhysicalDevice().getPhysicalDeviceRef(), &createInfo, nullptr, &device) )
	{
		throw std::runtime_error( "failed to create logical device!" );
	}

	vkGetDeviceQueue( device, indices.graphicsFamily.value(), 0, &graphicsQueue );
	vkGetDeviceQueue( device, indices.presentFamily.value(), 0, &presentQueue );
}

LogicalDevice::~LogicalDevice()
{
	vkDestroyDevice( device, nullptr );
}