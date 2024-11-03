#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <vector>

#include "device/Device.hpp"
#include "window/Window.hpp"

class Swapchain
{
public:
    Swapchain( Window& window, Device& device );
    ~Swapchain();

    operator VkSwapchainKHR&() { return swapchain; }
    operator VkFormat&() { return format; }
    operator VkExtent2D&() { return extent; }

    const uint32_t width() { return extent.width; }
    const uint32_t height() { return extent.height; }

    std::vector< VkImage >& getSwapchainImages() { return swapchainImages; }

private:
    VkSurfaceFormatKHR chooseSwapSurfaceFormat( const std::vector< VkSurfaceFormatKHR >& availableFormats );
    VkPresentModeKHR chooseSwapPresentMode( const std::vector< VkPresentModeKHR >& availablePresentModes );
    VkExtent2D chooseSwapExtent( const VkSurfaceCapabilitiesKHR& capabilities );

    Window& window;
    Device& device;

    VkSwapchainKHR swapchain;
    std::vector< VkImage > swapchainImages;
    VkFormat format;
    VkExtent2D extent;
};