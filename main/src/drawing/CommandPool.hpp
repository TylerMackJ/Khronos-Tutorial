#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "device/Device.hpp"

class CommandPool
{
public:
    CommandPool( Device& device );
    ~CommandPool();

    VkCommandPool& getCommandPool() { return commandPool; }

private:
    Device& device;
    VkCommandPool commandPool;
};