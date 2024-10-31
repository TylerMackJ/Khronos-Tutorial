#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <optional>

#include "PhysicalDevice.hpp"

class LogicalDevice
{
    friend class Device;

public:
    LogicalDevice(
        PhysicalDevice& physicalDevice,
        const std::vector< const char* > extensions,
        std::optional< const std::vector< const char* > > validationLayers = std::nullopt
    );
    ~LogicalDevice();

private:
    VkDevice& operator*() { return device; };
    operator VkDevice&() { return device; };

    VkQueue& getGraphicsQueue() { return graphicsQueue; }
    VkQueue& getPresentQueue() { return presentQueue; };

private:
    VkDevice device;
    VkQueue graphicsQueue;
    VkQueue presentQueue;
};
