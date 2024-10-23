#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

class TextureSampler
{
public:
    TextureSampler();
    ~TextureSampler();

    const VkSampler& getSampler() const { return sampler; }

private:
    VkSampler sampler;
};