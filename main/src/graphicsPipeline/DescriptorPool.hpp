#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "device/Device.hpp"

class DescriptorPool
{
public:
    DescriptorPool( Device& device, uint32_t maxFramesInFlight );
    ~DescriptorPool();

    operator VkDescriptorPool&() { return descriptorPool; }

private:
    Device& device;
    VkDescriptorPool descriptorPool;
};