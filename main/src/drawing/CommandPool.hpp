#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

class CommandPool
{
public:
    CommandPool();
    ~CommandPool();

    VkCommandPool& getCommandPool() { return commandPool; }

private:
    VkCommandPool commandPool;
};