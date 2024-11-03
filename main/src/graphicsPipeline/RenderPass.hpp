#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

class RenderPass
{
public:
    RenderPass( Device& device );
    ~RenderPass();

    operator VkRenderPass&() { return renderPass; }

private:
    Device& device;
    VkRenderPass renderPass;
};