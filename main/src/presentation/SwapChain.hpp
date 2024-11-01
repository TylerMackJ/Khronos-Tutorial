#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <vector>

#include "device/Device.hpp"
#include "window/Window.hpp"

class SwapChain
{
public:
    SwapChain( Window& window, Device& device );
    ~SwapChain();

    VkSwapchainKHR& getSwapChainRef() { return swapChain; }
    std::vector< VkImage >& getSwapChainImages() { return swapChainImages; }
    VkFormat& getSwapChainImageFormat() { return swapChainImageFormat; }
    VkExtent2D& getSwapChainExtent() { return swapChainExtent; }

private:
    VkSurfaceFormatKHR chooseSwapSurfaceFormat( const std::vector< VkSurfaceFormatKHR >& availableFormats );
    VkPresentModeKHR chooseSwapPresentMode( const std::vector< VkPresentModeKHR >& availablePresentModes );
    VkExtent2D chooseSwapExtent( const VkSurfaceCapabilitiesKHR& capabilities );

    Window& window;
    Device& device;

    VkSwapchainKHR swapChain;
    std::vector< VkImage > swapChainImages;
    VkFormat swapChainImageFormat;
    VkExtent2D swapChainExtent;
};