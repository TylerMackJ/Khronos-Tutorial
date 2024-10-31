#include "ImageView.hpp"

ImageView::ImageView(
    Device& device, VkImage image, VkFormat format, VkImageAspectFlags aspectFlags, uint32_t mipLevels
)
    : device( device )
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
    createInfo.subresourceRange.aspectMask = aspectFlags;
    createInfo.subresourceRange.baseMipLevel = 0;
    createInfo.subresourceRange.levelCount = mipLevels;
    createInfo.subresourceRange.baseArrayLayer = 0;
    createInfo.subresourceRange.layerCount = 1;

    if( vkCreateImageView( device, &createInfo, nullptr, &imageView ) != VK_SUCCESS )
    {
        throw std::runtime_error( "failed to create image view!" );
    }
}

ImageView::~ImageView() { vkDestroyImageView( device, imageView, nullptr ); }