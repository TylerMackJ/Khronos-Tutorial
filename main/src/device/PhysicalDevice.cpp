#include "PhysicalDevice.hpp"

#include <map>
#include <set>

#include "HelloTriangleApplication.hpp"

using App = HelloTriangleApplication;

PhysicalDevice::PhysicalDevice( Window& window, std::set< std::string > requiredExtensions )
    : window( window ), physicalDevice( VK_NULL_HANDLE ), msaaSamples( VK_SAMPLE_COUNT_1_BIT )
{
    uint32_t deviceCount = 0;
    vkEnumeratePhysicalDevices( window, &deviceCount, nullptr );
    if( deviceCount == 0 )
    {
        throw std::runtime_error( "failed to find GPUs with Vulkan support!" );
    }
    std::vector< VkPhysicalDevice > devices( deviceCount );
    vkEnumeratePhysicalDevices( window, &deviceCount, devices.data() );

    std::multimap< int, VkPhysicalDevice > candidates;

    for( const auto& device : devices )
    {
        int score = rateDeviceSuitability( device, requiredExtensions );
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

const QueueFamilyIndices PhysicalDevice::getQueueFamilyIndices() { return findQueueFamilies( physicalDevice, window ); }

const SwapChainSupportDetails PhysicalDevice::getSwapChainSupportDetails()
{
    return querySwapChainSupport( physicalDevice );
}

const VkFormat PhysicalDevice::findSupportedFormat(
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

int PhysicalDevice::rateDeviceSuitability( VkPhysicalDevice device, std::set< std::string >& requiredExtensions )
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

    bool extensionsSupported = checkDeviceExtensionSupport( device, requiredExtensions );
    bool swapChainAdequate = false;
    if( extensionsSupported )
    {
        SwapChainSupportDetails swapChainSupport = querySwapChainSupport( device );
        swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
    }

    if( !deviceFeatures.geometryShader || !findQueueFamilies( device, window ).isComplete() || !extensionsSupported ||
        !swapChainAdequate || !deviceFeatures.samplerAnisotropy )
    {
        return 0;
    }

    return score;
}

QueueFamilyIndices PhysicalDevice::findQueueFamilies( VkPhysicalDevice& device, Window& window )
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
        vkGetPhysicalDeviceSurfaceSupportKHR( device, i, window, &presentSupport );
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

bool PhysicalDevice::checkDeviceExtensionSupport( VkPhysicalDevice device, std::set< std::string >& requiredExtensions )
{
    uint32_t extensionCount;
    vkEnumerateDeviceExtensionProperties( device, nullptr, &extensionCount, nullptr );

    std::vector< VkExtensionProperties > availableExtensions( extensionCount );
    vkEnumerateDeviceExtensionProperties( device, nullptr, &extensionCount, availableExtensions.data() );

    for( const auto& extension : availableExtensions )
    {
        requiredExtensions.erase( extension.extensionName );
    }

    return requiredExtensions.empty();
}

SwapChainSupportDetails PhysicalDevice::querySwapChainSupport( VkPhysicalDevice device )
{
    SwapChainSupportDetails details;

    vkGetPhysicalDeviceSurfaceCapabilitiesKHR( device, window, &details.capabilities );

    uint32_t formatCount;
    vkGetPhysicalDeviceSurfaceFormatsKHR( device, window, &formatCount, nullptr );
    if( formatCount > 0 )
    {
        details.formats.resize( formatCount );
        vkGetPhysicalDeviceSurfaceFormatsKHR( device, window, &formatCount, details.formats.data() );
    }

    uint32_t presentModeCount;
    vkGetPhysicalDeviceSurfacePresentModesKHR( device, window, &presentModeCount, nullptr );
    if( presentModeCount > 0 )
    {
        details.presentModes.resize( presentModeCount );
        vkGetPhysicalDeviceSurfacePresentModesKHR( device, window, &presentModeCount, details.presentModes.data() );
    }

    return details;
}