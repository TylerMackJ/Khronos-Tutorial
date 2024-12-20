#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "device/Device.hpp"

class BufferMemoryMap
{
public:
    BufferMemoryMap(
        Device& device, VkDeviceMemory& bufferMemory, VkDeviceSize& offset, VkDeviceSize& size, VkMemoryMapFlags& flags
    );
    ~BufferMemoryMap();

    void copy( const void* src );

private:
    Device& device;
    VkDeviceMemory& bufferMemory;
    size_t size;
    void* data;
};