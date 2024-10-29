#include "PhysicalDevice.hpp"

#include <map>
#include <set>

#include "HelloTriangleApplication.hpp"

using App = HelloTriangleApplication;

PhysicalDevice::PhysicalDevice() : physicalDevice( VK_NULL_HANDLE ), msaaSamples( VK_SAMPLE_COUNT_1_BIT )
{
    uint32_t deviceCount = 0;
    vkEnumeratePhysicalDevices( App::get().getInstance().getInstanceRef(), &deviceCount, nullptr );
    if( deviceCount == 0 )
    {
        throw std::runtime_error( "failed to find GPUs with Vulkan support!" );
    }
    std::vector< VkPhysicalDevice > devices( deviceCount );
    vkEnumeratePhysicalDevices( App::get().getInstance().getInstanceRef(), &deviceCount, devices.data() );

    std::multimap< int, VkPhysicalDevice > candidates;

    for( const auto& device : devices )
    {
        int score = rateDeviceSuitability( device );
        candidates.insert( std::make_pair( score, device ) );
    }

    if( candidates.rbegin()->first > 0 )
    {
        physicalDevice = candidates.rbegin()->second;
    }
    else
    {
        throw std::runtime_error( "failed to find a suitable GPU!" );
    }

    vkGetPhysicalDeviceProperties( physicalDevice, &properties );

    // Get max usable sample count
    VkSampleCountFlags counts =
        properties.limits.framebufferColorSampleCounts & properties.limits.framebufferDepthSampleCounts;
    if( counts & VK_SAMPLE_COUNT_64_BIT )
    {
        msaaSamples = VK_SAMPLE_COUNT_64_BIT;
    }
    else if( counts & VK_SAMPLE_COUNT_32_BIT )
    {
        msaaSamples = VK_SAMPLE_COUNT_32_BIT;
    }
    else if( counts & VK_SAMPLE_COUNT_16_BIT )
    {
        msaaSamples = VK_SAMPLE_COUNT_16_BIT;
    }
    else if( counts & VK_SAMPLE_COUNT_8_BIT )
    {
        msaaSamples = VK_SAMPLE_COUNT_8_BIT;
    }
    else if( counts & VK_SAMPLE_COUNT_4_BIT )
    {
        msaaSamples = VK_SAMPLE_COUNT_4_BIT;
    }
    else if( counts & VK_SAMPLE_COUNT_2_BIT )
    {
        msaaSamples = VK_SAMPLE_COUNT_2_BIT;
    }
}

PhysicalDevice::QueueFamilyIndices PhysicalDevice::getQueueFamilyIndices()
{
    return findQueueFamilies( physicalDevice );
}

PhysicalDevice::SwapChainSupportDetails PhysicalDevice::getSwapChainSupportDetails()
{
    return querySwapChainSupport( physicalDevice );
}

VkFormat PhysicalDevice::findSupportedFormat(
    const std::vector< VkFormat >& candidates, VkImageTiling tiling, VkFormatFeatureFlags features
)
{
    for( VkFormat format : candidates )
    {
        VkFormatProperties props;
        vkGetPhysicalDeviceFormatProperties( physicalDevice, format, &props );

        if( tiling == VK_IMAGE_TILING_LINEAR && ( props.linearTilingFeatures & features ) == features )
        {
            return format;
        }
        else if( tiling == VK_IMAGE_TILING_OPTIMAL && ( props.optimalTilingFeatures & features ) == features )
        {
            return format;
        }
    }

    throw std::runtime_error( "failed to find supported format!" );
}

int PhysicalDevice::rateDeviceSuitability( VkPhysicalDevice device )
{
    VkPhysicalDeviceProperties deviceProperties;
    VkPhysicalDeviceFeatures deviceFeatures;
    vkGetPhysicalDeviceProperties( device, &deviceProperties );
    vkGetPhysicalDeviceFeatures( device, &deviceFeatures );

    int score = 0;

    if( deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU )
    {
        score += 1000;
    }

    score += deviceProperties.limits.maxImageDimension2D;

    bool extensionsSupported = checkDeviceExtensionSupport( device );
    bool swapChainAdequate = false;
    if( extensionsSupported )
    {
        SwapChainSupportDetails swapChainSupport = querySwapChainSupport( device );
        swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
    }

    if( !deviceFeatures.geometryShader || !findQueueFamilies( device ).isComplete() || !extensionsSupported ||
        !swapChainAdequate || !deviceFeatures.samplerAnisotropy )
    {
        return 0;
    }

    return score;
}

PhysicalDevice::QueueFamilyIndices PhysicalDevice::findQueueFamilies( VkPhysicalDevice device )
{
    QueueFamilyIndices indices;

    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties( device, &queueFamilyCount, nullptr );

    std::vector< VkQueueFamilyProperties > queueFamilies( queueFamilyCount );
    vkGetPhysicalDeviceQueueFamilyProperties( device, &queueFamilyCount, queueFamilies.data() );

    int i = 0;
    for( const auto& queueFamily : queueFamilies )
    {
        if( queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT )
        {
            indices.graphicsFamily = i;
        }
        VkBool32 presentSupport = false;
        vkGetPhysicalDeviceSurfaceSupportKHR( device, i, App::get().getSurface().getSurfaceRef(), &presentSupport );
        if( presentSupport )
        {
            indices.presentFamily = i;
        }
        if( indices.isComplete() )
        {
            break;
        }
        i++;
    }

    return indices;
}

bool PhysicalDevice::checkDeviceExtensionSupport( VkPhysicalDevice device )
{
    uint32_t extensionCount;
    vkEnumerateDeviceExtensionProperties( device, nullptr, &extensionCount, nullptr );

    std::vector< VkExtensionProperties > availableExtensions( extensionCount );
    vkEnumerateDeviceExtensionProperties( device, nullptr, &extensionCount, availableExtensions.data() );

    std::set< std::string > requiredExtensions(
        App::get().deviceExtensions.begin(), App::get().deviceExtensions.end()
    );

    for( const auto& extension : availableExtensions )
    {
        requiredExtensions.erase( extension.extensionName );
    }

    return requiredExtensions.empty();
}

PhysicalDevice::SwapChainSupportDetails PhysicalDevice::querySwapChainSupport( VkPhysicalDevice device )
{
    SwapChainSupportDetails details;

    vkGetPhysicalDeviceSurfaceCapabilitiesKHR( device, App::get().getSurface().getSurfaceRef(), &details.capabilities );

    uint32_t formatCount;
    vkGetPhysicalDeviceSurfaceFormatsKHR( device, App::get().getSurface().getSurfaceRef(), &formatCount, nullptr );
    if( formatCount > 0 )
    {
        details.formats.resize( formatCount );
        vkGetPhysicalDeviceSurfaceFormatsKHR(
            device, App::get().getSurface().getSurfaceRef(), &formatCount, details.formats.data()
        );
    }

    uint32_t presentModeCount;
    vkGetPhysicalDeviceSurfacePresentModesKHR(
        device, App::get().getSurface().getSurfaceRef(), &presentModeCount, nullptr
    );
    if( presentModeCount > 0 )
    {
        details.presentModes.resize( presentModeCount );
        vkGetPhysicalDeviceSurfacePresentModesKHR(
            device, App::get().getSurface().getSurfaceRef(), &presentModeCount, details.presentModes.data()
        );
    }

    return details;
}