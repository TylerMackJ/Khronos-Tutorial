#include "DescriptorPool.hpp"

#include <array>
#include <stdexcept>


DescriptorPool::DescriptorPool( Device& device, uint32_t maxFramesInFlight ) : device( device )
{
    std::array< VkDescriptorPoolSize, 2 > poolSizes{};
    poolSizes[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    poolSizes[0].descriptorCount = maxFramesInFlight;
    poolSizes[1].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    poolSizes[1].descriptorCount = maxFramesInFlight;

    VkDescriptorPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    poolInfo.poolSizeCount = static_cast< uint32_t >( poolSizes.size() );
    poolInfo.pPoolSizes = poolSizes.data();
    poolInfo.maxSets = maxFramesInFlight;

    if( vkCreateDescriptorPool( device, &poolInfo, nullptr, &descriptorPool ) != VK_SUCCESS )
    {
        throw std::runtime_error( "failed to create descriptor pool!" );
    }
}

DescriptorPool::~DescriptorPool() { vkDestroyDescriptorPool( device, descriptorPool, nullptr ); }