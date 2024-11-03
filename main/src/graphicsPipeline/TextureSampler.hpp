#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "device/Device.hpp"

class TextureSampler
{
public:
    TextureSampler( Device& device );
    ~TextureSampler();

    operator VkSampler&() { return sampler; }

private:
    Device& device;
    VkSampler sampler;
};