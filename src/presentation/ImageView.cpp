#include "ImageView.hpp"

#include "HelloTriangleApplication.hpp"

using App = HelloTriangleApplication;

ImageView::ImageView( VkImage image, VkFormat format )
{
    VkImageViewCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    createInfo.image = image;
    createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
    createInfo.format = format;
    createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
    createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
    createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
    createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
    createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    createInfo.subresourceRange.baseMipLevel = 0;
    createInfo.subresourceRange.levelCount = 1;
    createInfo.subresourceRange.baseArrayLayer = 0;
    createInfo.subresourceRange.layerCount = 1;

    if( vkCreateImageView( App::get().getLogicalDevice().getDeviceRef(), &createInfo, nullptr, &imageView ) !=
        VK_SUCCESS )
    {
        throw std::runtime_error( "failed to create image view!" );
    }
}

ImageView::~ImageView() { vkDestroyImageView( App::get().getLogicalDevice().getDeviceRef(), imageView, nullptr ); }