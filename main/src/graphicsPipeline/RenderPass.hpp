#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

class RenderPass
{
public:
    RenderPass( Device& device );
    ~RenderPass();

    VkRenderPass& getRenderPass() { return renderPass; }

private:
    Device& device;
    VkRenderPass renderPass;
};