#include "Swapchain.hpp"

#include <algorithm>

#include "device/PhysicalDevice.hpp"
#include "device/QueueFamilyIndices.hpp"
#include "device/SwapchainSupportDetails.hpp"

Swapchain::Swapchain( Window& window, Device& device )
    : window( window ), device( device ), swapchain( VK_NULL_HANDLE ), swapchainImages(), format( VK_FORMAT_UNDEFINED ),
      extent( { 0, 0 } )
{
    SwapchainSupportDetails swapchainSupport = device.getSwapchainSupportDetails();

    VkSurfaceFormatKHR surfaceFormat = chooseSwapSurfaceFormat( swapchainSupport.formats );
    VkPresentModeKHR presentMode = chooseSwapPresentMode( swapchainSupport.presentModes );

    format = surfaceFormat.format;
    extent = chooseSwapExtent( swapchainSupport.capabilities );

    uint32_t imageCount = swapchainSupport.capabilities.minImageCount + 1;
    if( swapchainSupport.capabilities.maxImageCount > 0 && imageCount > swapchainSupport.capabilities.maxImageCount )
    {
        imageCount = swapchainSupport.capabilities.maxImageCount;
    }

    VkSwapchainCreateInfoKHR createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    createInfo.surface = window;
    createInfo.minImageCount = imageCount;
    createInfo.imageFormat = format;
    createInfo.imageColorSpace = surfaceFormat.colorSpace;
    createInfo.imageExtent = extent;
    createInfo.imageArrayLayers = 1;
    createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

    QueueFamilyIndices indices = device.getQueueFamilyIndices();
    uint32_t queueFamilyIndices[] = { indices.graphicsFamily.value(), indices.presentFamily.value() };

    if( indices.graphicsFamily != indices.presentFamily )
    {
        createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
        createInfo.queueFamilyIndexCount = 2;
        createInfo.pQueueFamilyIndices = queueFamilyIndices;
    }
    {
        createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
        createInfo.queueFamilyIndexCount = 0;
        createInfo.pQueueFamilyIndices = nullptr;
    }
    createInfo.preTransform = swapchainSupport.capabilities.currentTransform;
    createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    createInfo.presentMode = presentMode;
    createInfo.clipped = VK_TRUE;
    createInfo.oldSwapchain = VK_NULL_HANDLE;

    if( vkCreateSwapchainKHR( device, &createInfo, nullptr, &swapchain ) != VK_SUCCESS )
    {
        throw std::runtime_error( "failed to create swap chain!" );
    }

    vkGetSwapchainImagesKHR( device, swapchain, &imageCount, nullptr );
    swapchainImages.resize( imageCount );
    vkGetSwapchainImagesKHR( device, swapchain, &imageCount, swapchainImages.data() );
}

Swapchain::~Swapchain() { vkDestroySwapchainKHR( device, swapchain, nullptr ); }

VkSurfaceFormatKHR Swapchain::chooseSwapSurfaceFormat( const std::vector< VkSurfaceFormatKHR >& availableFormats )
{
    for( const auto& availableFormat : availableFormats )
    {
        if( availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB &&
            availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR )
        {
            return availableFormat;
        }
    }

    return availableFormats[0];
}

VkPresentModeKHR Swapchain::chooseSwapPresentMode( const std::vector< VkPresentModeKHR >& availablePresentModes )
{
    for( const auto& availablePresentMode : availablePresentModes )
    {
        if( availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR )
        {
            return availablePresentMode;
        }
    }
    return VK_PRESENT_MODE_FIFO_KHR;
}

VkExtent2D Swapchain::chooseSwapExtent( const VkSurfaceCapabilitiesKHR& capabilities )
{
    if( capabilities.currentExtent.width != std::numeric_limits< uint32_t >::max() )
    {
        return capabilities.currentExtent;
    }
    else
    {
        int width, height;
        glfwGetFramebufferSize( window, &width, &height );

        VkExtent2D actualExtent = { static_cast< uint32_t >( width ), static_cast< uint32_t >( height ) };
        actualExtent.width =
            std::clamp( actualExtent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width );
        actualExtent.height =
            std::clamp( actualExtent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height );
        return actualExtent;
    }
}