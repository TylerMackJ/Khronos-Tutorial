#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

class SyncObjects
{
public:
    SyncObjects();
    ~SyncObjects();

    VkSemaphore& getImageAvailableSemaphore() { return imageAvailableSemaphore; }
    VkSemaphore& getRenderFinishedSemaphore() { return renderFinishedSemaphore; }
    VkFence& getInFlightFence() { return inFlightFence; }

private:
    VkSemaphore imageAvailableSemaphore;
    VkSemaphore renderFinishedSemaphore;
    VkFence inFlightFence;
};