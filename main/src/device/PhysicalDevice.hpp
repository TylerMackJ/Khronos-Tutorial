#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <optional>
#include <stdexcept>

#include "QueueFamilyIndices.hpp"
#include "SwapChainSupportDetails.hpp"
#include "window/Instance.hpp"
#include "window/Surface.hpp"


class PhysicalDevice
{
    friend class Device;
    friend class LogicalDevice;

public:
    PhysicalDevice();
    ~PhysicalDevice() = default;

private:
    VkPhysicalDevice& operator*() { return physicalDevice; }
    operator VkPhysicalDevice&() { return physicalDevice; }

    const QueueFamilyIndices getQueueFamilyIndices();
    const SwapChainSupportDetails getSwapChainSupportDetails();
    const VkPhysicalDeviceProperties& const getProperties() { return properties; }
    const VkFormat findSupportedFormat(
        const std::vector< VkFormat >& candidates, VkImageTiling tiling, VkFormatFeatureFlags features
    );
    const VkSampleCountFlagBits& const getMSAASamples() { return msaaSamples; }

private:
    int rateDeviceSuitability( VkPhysicalDevice device );
    static QueueFamilyIndices findQueueFamilies( VkPhysicalDevice device );
    bool checkDeviceExtensionSupport( VkPhysicalDevice device );
    SwapChainSupportDetails querySwapChainSupport( VkPhysicalDevice device );

    VkPhysicalDevice physicalDevice;
    VkPhysicalDeviceProperties properties;
    VkSampleCountFlagBits msaaSamples;
};
