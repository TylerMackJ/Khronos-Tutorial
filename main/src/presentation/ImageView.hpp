#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

class ImageView
{
public:
    ImageView( VkImage image, VkFormat format, VkImageAspectFlags aspectFlags, uint32_t mipLevels );
    ~ImageView();

    const VkImageView& const getImageView() { return imageView; }

private:
    VkImageView imageView;
};