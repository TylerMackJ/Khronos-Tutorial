#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <vector>

class SwapChain
{
public:
    SwapChain();
    ~SwapChain();

    VkSwapchainKHR& getSwapChainRef() { return swapChain; }
    std::vector<VkImage>& getSwapChainImages() { return swapChainImages; }
    VkFormat& getSwapChainImageFormat() { return swapChainImageFormat; }
    VkExtent2D& getSwapChainExtent() { return swapChainExtent; }

private:
	VkSurfaceFormatKHR chooseSwapSurfaceFormat( const std::vector<VkSurfaceFormatKHR>& availableFormats );
	VkPresentModeKHR chooseSwapPresentMode( const std::vector<VkPresentModeKHR>& availablePresentModes );
	VkExtent2D chooseSwapExtent( const VkSurfaceCapabilitiesKHR& capabilities );

    VkSwapchainKHR swapChain;
    std::vector<VkImage> swapChainImages;
    VkFormat swapChainImageFormat;
    VkExtent2D swapChainExtent;
};