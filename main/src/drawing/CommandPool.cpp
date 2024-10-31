#include "CommandPool.hpp"

#include "device/PhysicalDevice.hpp"
#include "device/QueueFamilyIndices.hpp"

CommandPool::CommandPool( Device& device ) : device( device )
{
    QueueFamilyIndices queueFamilyIndices = device.getQueueFamilyIndices();

    VkCommandPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
    poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily.value();

    if( vkCreateCommandPool( device, &poolInfo, nullptr, &commandPool ) != VK_SUCCESS )
    {
        throw std::runtime_error( "failed to create command pool!" );
    }
}

CommandPool::~CommandPool() { vkDestroyCommandPool( device, commandPool, nullptr ); }