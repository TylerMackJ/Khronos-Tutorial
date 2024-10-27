#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

class RenderPass
{
public:
    RenderPass();
    ~RenderPass();

    VkRenderPass& getRenderPass() { return renderPass; }

private:
    VkRenderPass renderPass;
};