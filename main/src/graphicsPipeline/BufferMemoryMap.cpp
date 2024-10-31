#include "BufferMemoryMap.hpp"

BufferMemoryMap::BufferMemoryMap(
    Device& device, VkDeviceMemory& bufferMemory, VkDeviceSize& offset, VkDeviceSize& size, VkMemoryMapFlags& flags
)
    : device( device ), bufferMemory( bufferMemory ), size( size ), data( nullptr )
{
    vkMapMemory( device, bufferMemory, offset, size, flags, &data );
}

BufferMemoryMap::~BufferMemoryMap() { vkUnmapMemory( device, bufferMemory ); }

void BufferMemoryMap::copy( const void* src ) { memcpy( data, src, size ); }