#include "DescriptorPool.hpp"

#include "HelloTriangleApplication.hpp"

#include <stdexcept>

using App = HelloTriangleApplication;

DescriptorPool::DescriptorPool()
{
    VkDescriptorPoolSize poolSize{};
    poolSize.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    poolSize.descriptorCount = App::get().MAX_FRAMES_IN_FLIGHT;

    VkDescriptorPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    poolInfo.poolSizeCount = 1;
    poolInfo.pPoolSizes = &poolSize;
    poolInfo.maxSets = App::get().MAX_FRAMES_IN_FLIGHT;

    if (vkCreateDescriptorPool(App::get().getLogicalDevice().getDeviceRef(), &poolInfo, nullptr, &descriptorPool) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create descriptor pool!");
    }
}

DescriptorPool::~DescriptorPool()
{
    vkDestroyDescriptorPool(App::get().getLogicalDevice().getDeviceRef(), descriptorPool, nullptr);
}