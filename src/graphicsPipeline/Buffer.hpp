#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

class Buffer
{
public:
    Buffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties);
    ~Buffer();

    const VkBuffer& getBuffer() const { return buffer; }
    const VkDeviceMemory& getBufferMemory() const { return bufferMemory; }

    static void copyBuffer(Buffer& srcBuffer, Buffer& dstBuffer, VkDeviceSize size);

private:
    static uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);

    VkBuffer buffer;
    VkDeviceMemory bufferMemory;
};