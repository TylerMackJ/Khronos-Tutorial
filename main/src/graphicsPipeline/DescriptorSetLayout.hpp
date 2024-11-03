#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#define GLM_FORCE_DEFAULT_ALIGNED_GENTYPES
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

#include "device/Device.hpp"

class DescriptorSetLayout
{
public:
    DescriptorSetLayout( Device& device );
    ~DescriptorSetLayout();

    operator VkDescriptorSetLayout&() { return descriptorSetLayout; }
    operator VkDescriptorSetLayout*() { return &descriptorSetLayout; }

private:
    Device& device;
    VkDescriptorSetLayout descriptorSetLayout;
};