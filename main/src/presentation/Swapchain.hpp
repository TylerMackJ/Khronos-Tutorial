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

    VkSwapchainKHR& getSwapchainRef() { return swapchain; }
    std::vector< VkImage >& getSwapchainImages() { return swapchainImages; }
    VkFormat& getSwapchainImageFormat() { return swapchainImageFormat; }
    VkExtent2D& getSwapchainExtent() { return swapchainExtent; }

private:
    VkSurfaceFormatKHR chooseSwapSurfaceFormat( const std::vector< VkSurfaceFormatKHR >& availableFormats );
    VkPresentModeKHR chooseSwapPresentMode( const std::vector< VkPresentModeKHR >& availablePresentModes );
    VkExtent2D chooseSwapExtent( const VkSurfaceCapabilitiesKHR& capabilities );

    Window& window;
    Device& device;

    VkSwapchainKHR swapchain;
    std::vector< VkImage > swapchainImages;
    VkFormat swapchainImageFormat;
    VkExtent2D swapchainExtent;
};