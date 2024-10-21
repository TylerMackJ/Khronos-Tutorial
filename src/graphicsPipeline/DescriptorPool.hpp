#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

class DescriptorPool
{
public:
    DescriptorPool();
    ~DescriptorPool();

    const VkDescriptorPool& getDescriptorPool() const { return descriptorPool; }
private:
    VkDescriptorPool descriptorPool;
};