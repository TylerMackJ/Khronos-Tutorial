#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

class BufferMemoryMap
{
public:
    BufferMemoryMap( VkDeviceMemory& bufferMemory, VkDeviceSize& offset, VkDeviceSize& size, VkMemoryMapFlags& flags );
    ~BufferMemoryMap();

    void copy( const void* src );

private:
    VkDeviceMemory& bufferMemory;
    size_t size;
    void* data;
};