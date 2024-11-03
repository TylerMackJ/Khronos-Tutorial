#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <optional>
#include <set>
#include <stdexcept>

#include "QueueFamilyIndices.hpp"
#include "SwapchainSupportDetails.hpp"
#include "window/Window.hpp"

class PhysicalDevice
{
    friend class Device;
    friend class LogicalDevice;

public:
    PhysicalDevice( Window& window, std::set< std::string > requiredExtensions );
    ~PhysicalDevice() = default;

private:
    VkPhysicalDevice& operator*() { return physicalDevice; }
    operator VkPhysicalDevice&() { return physicalDevice; }

    const QueueFamilyIndices getQueueFamilyIndices();
    const SwapchainSupportDetails getSwapchainSupportDetails();
    const VkPhysicalDeviceProperties& const getProperties() { return properties; }
    const VkFormat findSupportedFormat(
        const std::vector< VkFormat >& candidates, VkImageTiling tiling, VkFormatFeatureFlags features
    );
    const VkSampleCountFlagBits& const getMSAASamples() { return msaaSamples; }

private:
    int rateDeviceSuitability( VkPhysicalDevice device, std::set< std::string >& requiredExtensions );
    static QueueFamilyIndices findQueueFamilies( VkPhysicalDevice& device, Window& window );
    bool checkDeviceExtensionSupport( VkPhysicalDevice device, std::set< std::string >& requiredExtensions );
    SwapchainSupportDetails querySwapchainSupport( VkPhysicalDevice device );

    Window& window;

    VkPhysicalDevice physicalDevice;
    VkPhysicalDeviceProperties properties;
    VkSampleCountFlagBits msaaSamples;
};
