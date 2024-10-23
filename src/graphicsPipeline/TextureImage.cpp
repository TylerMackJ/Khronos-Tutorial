#include "TextureImage.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "HelloTriangleApplication.hpp"

#include <stdexcept>

#include "drawing/CommandBuffer.hpp"

using App = HelloTriangleApplication;

TextureImage::TextureImage(
    const char* filename,
    VkFormat format,
    VkImageTiling tiling,
    VkImageUsageFlags usage,
    VkMemoryPropertyFlags properties
)
    : format( format )
{
    int texWidth, texHeight, texChannels;
    stbi_uc* pixels = stbi_load( filename, &texWidth, &texHeight, &texChannels, STBI_rgb_alpha );
    VkDeviceSize imageSize = texWidth * texHeight * 4;

    width = static_cast<uint32_t>( texWidth );
    height = static_cast<uint32_t>( texHeight );

    if( !pixels )
    {
        throw std::runtime_error( "failed to load texture image!" );
    }

    std::unique_ptr<Buffer> stagingBuffer = std::make_unique<Buffer>(
        imageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
    );
    stagingBuffer->mapMemory( 0, imageSize, 0 );
    stagingBuffer->copyTo( pixels );
    stagingBuffer->unmapMemory();

    stbi_image_free( pixels );

    VkImageCreateInfo imageInfo{};
    imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    imageInfo.imageType = VK_IMAGE_TYPE_2D;
    imageInfo.extent.width = width;
    imageInfo.extent.height = height;
    imageInfo.extent.depth = 1;
    imageInfo.mipLevels = 1;
    imageInfo.arrayLayers = 1;
    imageInfo.format = format;
    imageInfo.tiling = tiling;
    imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    imageInfo.usage = usage;
    imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
    imageInfo.flags = 0;

    if( vkCreateImage( App::get().getLogicalDevice().getDeviceRef(), &imageInfo, nullptr, &textureImage ) !=
        VK_SUCCESS )
    {
        throw std::runtime_error( "failed to create texture image!" );
    }

    VkMemoryRequirements memRequirements;
    vkGetImageMemoryRequirements( App::get().getLogicalDevice().getDeviceRef(), textureImage, &memRequirements );

    VkMemoryAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = Buffer::findMemoryType( memRequirements.memoryTypeBits, properties );

    if( vkAllocateMemory( App::get().getLogicalDevice().getDeviceRef(), &allocInfo, nullptr, &textureImageMemory ) !=
        VK_SUCCESS )
    {
        throw std::runtime_error( "failed to allocate texture image memory!" );
    }

    vkBindImageMemory( App::get().getLogicalDevice().getDeviceRef(), textureImage, textureImageMemory, 0 );

    transitionLayout( VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL );
    copyBufferToImage( *stagingBuffer );
    transitionLayout( VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL );
}

TextureImage::~TextureImage()
{
    vkDestroyImage( App::get().getLogicalDevice().getDeviceRef(), textureImage, nullptr );
    vkFreeMemory( App::get().getLogicalDevice().getDeviceRef(), textureImageMemory, nullptr );
}

void TextureImage::transitionLayout( VkImageLayout oldLayout, VkImageLayout newLayout )
{
    VkCommandBuffer commandBuffer = CommandBuffer::beginSingleTimeCommands();

    VkImageMemoryBarrier barrier{};
    barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
    barrier.oldLayout = oldLayout;
    barrier.newLayout = newLayout;
    barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.image = textureImage;
    barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    barrier.subresourceRange.baseMipLevel = 0;
    barrier.subresourceRange.levelCount = 1;
    barrier.subresourceRange.baseArrayLayer = 0;
    barrier.subresourceRange.layerCount = 1;

    VkPipelineStageFlags sourceStage;
    VkPipelineStageFlags destinationStage;

    if( oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL )
    {
        barrier.srcAccessMask = 0;
        barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
        sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
        destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
    }
    else if( oldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL &&
             newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL )
    {
        barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
        barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
        sourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
        destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
    }
    else
    {
        throw std::invalid_argument( "unsupported layout transition!" );
    }

    vkCmdPipelineBarrier( commandBuffer, sourceStage, destinationStage, 0, 0, nullptr, 0, nullptr, 1, &barrier );

    CommandBuffer::endSingleTimeCommands( commandBuffer );
}

void TextureImage::copyBufferToImage( Buffer& buffer )
{
    VkCommandBuffer commandBuffer = CommandBuffer::beginSingleTimeCommands();

    VkBufferImageCopy region{};
    region.bufferOffset = 0;
    region.bufferRowLength = 0;
    region.bufferImageHeight = 0;
    region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    region.imageSubresource.mipLevel = 0;
    region.imageSubresource.baseArrayLayer = 0;
    region.imageSubresource.layerCount = 1;
    region.imageOffset = { 0, 0, 0 };
    region.imageExtent = { width, height, 1 };

    vkCmdCopyBufferToImage(
        commandBuffer, buffer.getBuffer(), textureImage, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &region
    );

    CommandBuffer::endSingleTimeCommands( commandBuffer );
}

void TextureImage::createImageView() { textureImageView = std::make_unique<ImageView>( textureImage, format ); }