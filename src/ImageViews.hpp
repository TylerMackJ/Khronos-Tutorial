#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <vector>

class ImageViews
{
public:
    ImageViews();
    ~ImageViews();

    std::vector<VkImageView>& getImageViews() { return swapChainImageViews; }

private:
    std::vector<VkImageView> swapChainImageViews;
};