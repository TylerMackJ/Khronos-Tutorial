#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

class ImageView
{
public:
    ImageView( VkImage image, VkFormat format, VkImageAspectFlags aspectFlags );
    ~ImageView();

    const VkImageView& const getImageView() { return imageView; }

private:
    VkImageView imageView;
};