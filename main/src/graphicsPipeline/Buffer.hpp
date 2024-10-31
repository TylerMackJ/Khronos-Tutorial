#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <memory>

#include "BufferMemoryMap.hpp"
#include "device/Device.hpp"

class Buffer
{
public:
    Buffer( Device& device, VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties );
    ~Buffer();

    const VkBuffer& getBuffer() const { return buffer; }
    const VkDeviceMemory& getBufferMemory() const { return bufferMemory; }

    void mapMemory( VkDeviceSize offset, VkDeviceSize size, VkMemoryMapFlags flags );
    void copyTo( const void* data );
    void unmapMemory();

    static void copyBuffer( Device& device, Buffer& srcBuffer, Buffer& dstBuffer, VkDeviceSize size );
    static uint32_t findMemoryType( Device& device, uint32_t typeFilter, VkMemoryPropertyFlags properties );

private:
    Device& device;
    VkBuffer buffer;
    VkDeviceMemory bufferMemory;
    std::unique_ptr< BufferMemoryMap > bufferMemoryMap;
};