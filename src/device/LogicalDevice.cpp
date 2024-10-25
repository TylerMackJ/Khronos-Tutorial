#include "LogicalDevice.hpp"

#include <set>

#include "HelloTriangleApplication.hpp"

using App = HelloTriangleApplication;

LogicalDevice::LogicalDevice()
{
    PhysicalDevice::QueueFamilyIndices indices = App::get().getPhysicalDevice().getQueueFamilyIndices();

    std::vector< VkDeviceQueueCreateInfo > queueCreateInfos;
    std::set< uint32_t > uniqueQueueFamilies = { indices.graphicsFamily.value(), indices.presentFamily.value() };

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
    deviceFeatures.samplerAnisotropy = VK_TRUE;

    VkDeviceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    createInfo.pQueueCreateInfos = queueCreateInfos.data();
    createInfo.queueCreateInfoCount = static_cast< uint32_t >( queueCreateInfos.size() );
    createInfo.pEnabledFeatures = &deviceFeatures;
    createInfo.enabledExtensionCount = static_cast< uint32_t >( App::get().deviceExtensions.size() );
    createInfo.ppEnabledExtensionNames = App::get().deviceExtensions.data();
    if( App::get().enableValidationLayers )
    {
        createInfo.enabledLayerCount = static_cast< uint32_t >( App::get().validationLayers.size() );
        createInfo.ppEnabledLayerNames = App::get().validationLayers.data();
    }
    else
    {
        createInfo.enabledLayerCount = 0;
    }

    if( vkCreateDevice( App::get().getPhysicalDevice().getPhysicalDeviceRef(), &createInfo, nullptr, &device ) )
    {
        throw std::runtime_error( "failed to create logical device!" );
    }

    vkGetDeviceQueue( device, indices.graphicsFamily.value(), 0, &graphicsQueue );
    vkGetDeviceQueue( device, indices.presentFamily.value(), 0, &presentQueue );
}

LogicalDevice::~LogicalDevice() { vkDestroyDevice( device, nullptr ); }