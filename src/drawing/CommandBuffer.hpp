#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <vector>

class CommandBuffer
{
public:
    CommandBuffer();
    ~CommandBuffer();

    void recordCommandBuffer( VkCommandBuffer commandBuffer, uint32_t imageIndex );

    std::vector< VkCommandBuffer >& getCommandBuffers() { return commandBuffers; }

    static VkCommandBuffer beginSingleTimeCommands();
    static void endSingleTimeCommands( VkCommandBuffer commandBuffer );

private:
    std::vector< VkCommandBuffer > commandBuffers;
};