#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <vector>

class GraphicsPipeline
{
public:
    GraphicsPipeline( Device& device );
    ~GraphicsPipeline();

    operator VkPipeline&() { return graphicsPipeline; }
    operator VkPipelineLayout&() { return pipelineLayout; }

private:
    VkShaderModule createShaderModule( const std::vector< char >& code );

    Device& device;

    VkPipeline graphicsPipeline;
    VkPipelineLayout pipelineLayout;
    VkShaderModule vertShaderModule;
    VkShaderModule fragShaderModule;
};