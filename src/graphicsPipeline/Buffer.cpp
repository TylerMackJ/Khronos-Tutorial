#include "Buffer.hpp"

#include "HelloTriangleApplication.hpp"

using App = HelloTriangleApplication;

Buffer::Buffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties)
{
    VkBufferCreateInfo bufferInfo{};
    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.size = size;
    bufferInfo.usage = usage;
    bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    if (vkCreateBuffer(App::get().getLogicalDevice().getDeviceRef(), &bufferInfo, nullptr, &buffer) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create buffer!");
    }

    VkMemoryRequirements memRequirements;
    vkGetBufferMemoryRequirements(App::get().getLogicalDevice().getDeviceRef(), buffer, &memRequirements);

    VkMemoryAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits, properties);

    if (vkAllocateMemory(App::get().getLogicalDevice().getDeviceRef(), &allocInfo, nullptr, &bufferMemory) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to allocate vertex buffer memory!");
    }

    vkBindBufferMemory(App::get().getLogicalDevice().getDeviceRef(), buffer, bufferMemory, 0);

}

Buffer::~Buffer()
{
    vkDestroyBuffer(App::get().getLogicalDevice().getDeviceRef(), buffer, nullptr);
    vkFreeMemory(App::get().getLogicalDevice().getDeviceRef(), bufferMemory, nullptr);
}

void Buffer::mapMemory( VkDeviceSize offset, VkDeviceSize size, VkMemoryMapFlags flags )
{
    bufferMemoryMap = std::make_unique<BufferMemoryMap>(bufferMemory, offset, size, flags);
}

void Buffer::copyTo( const void* data )
{
    if( bufferMemoryMap == nullptr )
    {
        std::runtime_error("Buffer not mapped!");
    }
    bufferMemoryMap->copy(data);
}

void Buffer::unmapMemory()
{
    bufferMemoryMap.reset();
}


uint32_t Buffer::findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties)
{
    VkPhysicalDeviceMemoryProperties memProperties;
    vkGetPhysicalDeviceMemoryProperties(App::get().getPhysicalDevice().getPhysicalDeviceRef(), &memProperties);
    for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++)
    {
        if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties)
        {
            return i;
        }
    }

    throw std::runtime_error("failed to find suitable memory type!");
}

void Buffer::copyBuffer(Buffer& srcBuffer, Buffer& dstBuffer, VkDeviceSize size)
{
    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandPool = App::get().getCommandPool().getCommandPool();
    allocInfo.commandBufferCount = 1;

    VkCommandBuffer commandBuffer;
    vkAllocateCommandBuffers(App::get().getLogicalDevice().getDeviceRef(), &allocInfo, &commandBuffer);

    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
    vkBeginCommandBuffer(commandBuffer, &beginInfo);

    VkBufferCopy copyRegion{};
    copyRegion.srcOffset = 0;
    copyRegion.dstOffset = 0;
    copyRegion.size = size;
    vkCmdCopyBuffer(commandBuffer, srcBuffer.getBuffer(), dstBuffer.getBuffer(), 1, &copyRegion);

    vkEndCommandBuffer(commandBuffer);

    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &commandBuffer;

    vkQueueSubmit(App::get().getLogicalDevice().getGraphicsQueueRef(), 1, &submitInfo, VK_NULL_HANDLE);
    vkQueueWaitIdle(App::get().getLogicalDevice().getGraphicsQueueRef());

    vkFreeCommandBuffers(App::get().getLogicalDevice().getDeviceRef(), App::get().getCommandPool().getCommandPool(), 1, &commandBuffer);
}