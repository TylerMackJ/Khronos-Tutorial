#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <vector>

class Framebuffers
{
public:
    Framebuffers();
    ~Framebuffers();

    std::vector< VkFramebuffer >& getFramebuffers() { return swapChainFramebuffers; }

private:
    std::vector< VkFramebuffer > swapChainFramebuffers;
};