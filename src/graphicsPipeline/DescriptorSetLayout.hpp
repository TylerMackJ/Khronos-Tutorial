#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#define GLM_FORCE_DEFAULT_ALIGNED_GENTYPES
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm.hpp>

class DescriptorSetLayout
{
public:
    DescriptorSetLayout();
    ~DescriptorSetLayout();

    const VkDescriptorSetLayout& getDescriptorSetLayout() const { return descriptorSetLayout; }

private:
    VkDescriptorSetLayout descriptorSetLayout;
};