#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <vector>

#include "device/Device.hpp"

class Framebuffers
{
public:
    Framebuffers( Device& device );
    ~Framebuffers();

    std::vector< VkFramebuffer >& getFramebuffers() { return swapchainFramebuffers; }

private:
    Device& device;
    std::vector< VkFramebuffer > swapchainFramebuffers;
};