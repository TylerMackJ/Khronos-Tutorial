#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "Buffer.hpp"
#include "presentation/ImageView.hpp"

class TextureImage
{
public:
    TextureImage(
        const char* filename,
        VkFormat format,
        VkImageTiling tiling,
        VkImageUsageFlags usage,
        VkMemoryPropertyFlags properties
    );
    ~TextureImage();

    const VkImage& getImage() const { return textureImage; }
    const VkFormat& getFormat() const { return format; }
    const VkImageView& getImageView() const { return textureImageView->getImageView(); }

    void transitionLayout( VkImageLayout oldLayout, VkImageLayout newLayout );
    void copyBufferToImage( Buffer& buffer );
    void createImageView();

private:
    VkImage textureImage;
    VkDeviceMemory textureImageMemory;
    VkFormat format;
    uint32_t width;
    uint32_t height;
    std::unique_ptr<Buffer> stagingBuffer;
    std::unique_ptr<ImageView> textureImageView;
};