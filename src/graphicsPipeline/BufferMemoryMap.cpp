#include "BufferMemoryMap.hpp"

#include "HelloTriangleApplication.hpp"

using App = HelloTriangleApplication;

BufferMemoryMap::BufferMemoryMap(
    VkDeviceMemory& bufferMemory, VkDeviceSize& offset, VkDeviceSize& size, VkMemoryMapFlags& flags
)
    : bufferMemory( bufferMemory ), size( size ), data( nullptr )
{
    vkMapMemory( App::get().getLogicalDevice().getDeviceRef(), bufferMemory, offset, size, flags, &data );
}

BufferMemoryMap::~BufferMemoryMap() { vkUnmapMemory( App::get().getLogicalDevice().getDeviceRef(), bufferMemory ); }

void BufferMemoryMap::copy( const void* src ) { memcpy( data, src, size ); }