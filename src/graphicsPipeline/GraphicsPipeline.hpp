#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <vector>

class GraphicsPipeline
{
public:
    GraphicsPipeline();
    ~GraphicsPipeline();

    const VkPipeline& getPipeline() const { return graphicsPipeline; }
    const VkPipelineLayout& getPipelineLayout() const { return pipelineLayout; }

private:
    VkShaderModule createShaderModule( const std::vector< char >& code );

    VkPipeline graphicsPipeline;
    VkPipelineLayout pipelineLayout;
    VkShaderModule vertShaderModule;
    VkShaderModule fragShaderModule;
};