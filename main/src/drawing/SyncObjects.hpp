#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <vector>

#include "device/Device.hpp"

class SyncObjects
{
public:
    SyncObjects( Device& device );
    ~SyncObjects();

    std::vector< VkSemaphore >& getImageAvailableSemaphores() { return imageAvailableSemaphores; }
    std::vector< VkSemaphore >& getRenderFinishedSemaphores() { return renderFinishedSemaphores; }
    std::vector< VkFence >& getInFlightFences() { return inFlightFences; }

private:
    Device& device;
    std::vector< VkSemaphore > imageAvailableSemaphores;
    std::vector< VkSemaphore > renderFinishedSemaphores;
    std::vector< VkFence > inFlightFences;
};