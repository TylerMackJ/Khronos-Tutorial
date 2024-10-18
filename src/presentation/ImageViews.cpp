#include "ImageViews.hpp"

#include "HelloTriangleApplication.hpp"

ImageViews::ImageViews()
{
    swapChainImageViews.resize( HelloTriangleApplication::getSwapChain().getSwapChainImages().size() ); 

    for( size_t i = 0; i < swapChainImageViews.size(); i++ )
    {
        VkImageViewCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        createInfo.image = HelloTriangleApplication::getSwapChain().getSwapChainImages()[i];
        createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
        createInfo.format = HelloTriangleApplication::getSwapChain().getSwapChainImageFormat();
        createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        createInfo.subresourceRange.baseMipLevel = 0;
        createInfo.subresourceRange.levelCount = 1;
        createInfo.subresourceRange.baseArrayLayer = 0;
        createInfo.subresourceRange.layerCount = 1;

        if( vkCreateImageView( HelloTriangleApplication::getLogicalDevice().getDeviceRef(), &createInfo, nullptr, &swapChainImageViews[i] ) != VK_SUCCESS )
        {
            throw std::runtime_error( "failed to create image views!" );
        }
    }
}

ImageViews::~ImageViews()
{
    for( auto imageView : swapChainImageViews )
    {
        vkDestroyImageView( HelloTriangleApplication::getLogicalDevice().getDeviceRef(), imageView, nullptr );
    }
}