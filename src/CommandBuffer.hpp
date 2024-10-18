#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <vector>

class CommandBuffer
{
public:
    CommandBuffer();
    ~CommandBuffer();

    void recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex);

    std::vector<VkCommandBuffer>& getCommandBuffers() { return commandBuffers; }

private:
    std::vector<VkCommandBuffer> commandBuffers;
};