#pragma once

#include <memory>
#include <set>
#include <vector>

#include "LogicalDevice.hpp"
#include "PhysicalDevice.hpp"
#include "QueueFamilyIndices.hpp"
#include "SwapchainSupportDetails.hpp"
#include "window/Window.hpp"

class Device
{
public:
    Device(
        Window& window,
        std::vector< const char* > deviceExtensions,
        std::optional< const std::vector< const char* > > validationLayers = std::nullopt
    );
    ~Device() = default;

    operator VkPhysicalDevice&() { return *physicalDevice; }
    operator VkDevice&() { return *logicalDevice; }

    PhysicalDevice& getPhysicalDevice() { return *physicalDevice; }
    LogicalDevice& getLogicalDevice() { return *logicalDevice; }

    const QueueFamilyIndices getQueueFamilyIndices() { return physicalDevice->getQueueFamilyIndices(); }
    const SwapchainSupportDetails getSwapchainSupportDetails() { return physicalDevice->getSwapchainSupportDetails(); };
    const VkSampleCountFlagBits& const getMSAASamples() { return physicalDevice->getMSAASamples(); }
    const VkFormat findSupportedFormat(
        const std::vector< VkFormat >& candidates, VkImageTiling tiling, VkFormatFeatureFlags features
    )
    {
        return physicalDevice->findSupportedFormat( candidates, tiling, features );
    }
    const VkPhysicalDeviceProperties& const getProperties() { return physicalDevice->getProperties(); }

    VkQueue& getGraphicsQueue() { return logicalDevice->getGraphicsQueue(); }
    VkQueue& getPresentQueue() { return logicalDevice->getPresentQueue(); };

private:
    std::unique_ptr< PhysicalDevice > physicalDevice;
    std::unique_ptr< LogicalDevice > logicalDevice;
};
