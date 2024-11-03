#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <vector>

#include "CommandPool.hpp"
#include "device/Device.hpp"

class CommandBuffer
{
public:
    CommandBuffer( Device& Device, CommandPool& commandPool, uint32_t maxFramesInFlight );
    ~CommandBuffer();

    void recordCommandBuffer( VkCommandBuffer commandBuffer, uint32_t imageIndex );

    std::vector< VkCommandBuffer >& getCommandBuffers() { return commandBuffers; }

    static VkCommandBuffer beginSingleTimeCommands( Device& device );
    static void endSingleTimeCommands( Device& device, VkCommandBuffer commandBuffer );

private:
    Device& device;

    std::vector< VkCommandBuffer > commandBuffers;
};