#include "Buffer.hpp"

#include "drawing/CommandBuffer.hpp"

Buffer::Buffer( Device& device, VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties )
    : device( device )
{
    VkBufferCreateInfo bufferInfo{};
    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.size = size;
    bufferInfo.usage = usage;
    bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    if( vkCreateBuffer( device, &bufferInfo, nullptr, &buffer ) != VK_SUCCESS )
    {
        throw std::runtime_error( "failed to create buffer!" );
    }

    VkMemoryRequirements memRequirements;
    vkGetBufferMemoryRequirements( device, buffer, &memRequirements );

    VkMemoryAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = findMemoryType( device, memRequirements.memoryTypeBits, properties );

    if( vkAllocateMemory( device, &allocInfo, nullptr, &bufferMemory ) != VK_SUCCESS )
    {
        throw std::runtime_error( "failed to allocate vertex buffer memory!" );
    }

    vkBindBufferMemory( device, buffer, bufferMemory, 0 );
}

Buffer::~Buffer()
{
    vkDestroyBuffer( device, buffer, nullptr );
    vkFreeMemory( device, bufferMemory, nullptr );
}

void Buffer::mapMemory( VkDeviceSize offset, VkDeviceSize size, VkMemoryMapFlags flags )
{
    bufferMemoryMap = std::make_unique< BufferMemoryMap >( device, bufferMemory, offset, size, flags );
}

void Buffer::copyTo( const void* data )
{
    if( bufferMemoryMap == nullptr )
    {
        std::runtime_error( "Buffer not mapped!" );
    }
    bufferMemoryMap->copy( data );
}

void Buffer::unmapMemory() { bufferMemoryMap.reset(); }

uint32_t Buffer::findMemoryType( Device& device, uint32_t typeFilter, VkMemoryPropertyFlags properties )
{
    VkPhysicalDeviceMemoryProperties memProperties;
    vkGetPhysicalDeviceMemoryProperties( device, &memProperties );
    for( uint32_t i = 0; i < memProperties.memoryTypeCount; i++ )
    {
        if( ( typeFilter & ( 1 << i ) ) && ( memProperties.memoryTypes[i].propertyFlags & properties ) == properties )
        {
            return i;
        }
    }

    throw std::runtime_error( "failed to find suitable memory type!" );
}

void Buffer::copyBuffer( Device& device, Buffer& srcBuffer, Buffer& dstBuffer, VkDeviceSize size )
{
    VkCommandBuffer commandBuffer = CommandBuffer::beginSingleTimeCommands( device );

    VkBufferCopy copyRegion{};
    copyRegion.srcOffset = 0;
    copyRegion.dstOffset = 0;
    copyRegion.size = size;
    vkCmdCopyBuffer( commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion );

    CommandBuffer::endSingleTimeCommands( device, commandBuffer );
}