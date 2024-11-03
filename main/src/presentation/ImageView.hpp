#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "device/Device.hpp"

class ImageView
{
public:
    ImageView( Device& device, VkImage image, VkFormat format, VkImageAspectFlags aspectFlags, uint32_t mipLevels );
    ~ImageView();

    operator VkImageView&() { return imageView; }

private:
    Device& device;
    VkImageView imageView;
};