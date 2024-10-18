#include "CommandPool.hpp"

#include "HelloTriangleApplication.hpp"
#include "device/PhysicalDevice.hpp"

CommandPool::CommandPool()
{
    PhysicalDevice::QueueFamilyIndices queueFamilyIndices = HelloTriangleApplication::getPhysicalDevice().getQueueFamilyIndices(); 

    VkCommandPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
    poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily.value();

    if (vkCreateCommandPool(HelloTriangleApplication::getLogicalDevice().getDeviceRef(), &poolInfo, nullptr, &commandPool) != VK_SUCCESS) 
    {
        throw std::runtime_error("failed to create command pool!");
    }
}

CommandPool::~CommandPool()
{
    vkDestroyCommandPool(HelloTriangleApplication::getLogicalDevice().getDeviceRef(), commandPool, nullptr);
}