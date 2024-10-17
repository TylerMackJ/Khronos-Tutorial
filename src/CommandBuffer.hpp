#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

class CommandBuffer
{
public:
    CommandBuffer();
    ~CommandBuffer();

    void recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex);

    VkCommandBuffer& getCommandBuffer() { return commandBuffer; }

private:
    VkCommandBuffer commandBuffer;
};