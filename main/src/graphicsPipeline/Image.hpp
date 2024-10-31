#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "Buffer.hpp"
#include "device/Device.hpp"
#include "presentation/ImageView.hpp"

class Image
{
public:
    Image(
        Device& device,
        uint32_t width,
        uint32_t height,
        uint32_t mipLevels,
        VkSampleCountFlagBits numSamples,
        VkFormat format,
        VkImageTiling tiling,
        VkImageUsageFlags usage,
        VkMemoryPropertyFlags properties
    );
    Image(
        Device& device,
        const char* filename,
        VkSampleCountFlagBits numSamples,
        VkFormat format,
        VkImageTiling tiling,
        VkImageUsageFlags usage,
        VkMemoryPropertyFlags properties
    );
    ~Image();

    const VkImage& getImage() const { return image; }
    const VkFormat& getFormat() const { return format; }
    const VkImageView& getImageView() const { return imageView->getImageView(); }

    void transitionLayout( VkImageLayout oldLayout, VkImageLayout newLayout );
    void copyBufferToImage( Buffer& buffer );
    void createImageView( VkImageAspectFlags aspectFlags );

private:
    void createImage();
    void generateMipmaps();

    static bool hasStencilComponent( VkFormat format );

    Device& device;
    VkImage image;
    VkDeviceMemory imageMemory;
    uint32_t mipLevels;
    VkSampleCountFlagBits numSamples;
    uint32_t width;
    uint32_t height;
    VkFormat format;
    VkImageTiling tiling;
    VkImageUsageFlags usage;
    VkMemoryPropertyFlags properties;
    std::unique_ptr< Buffer > stagingBuffer;
    std::unique_ptr< ImageView > imageView;
};