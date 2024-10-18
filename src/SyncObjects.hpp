#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <vector>

class SyncObjects
{
public:
    SyncObjects();
    ~SyncObjects();

    std::vector<VkSemaphore>& getImageAvailableSemaphores() { return imageAvailableSemaphores; }
    std::vector<VkSemaphore>& getRenderFinishedSemaphores() { return renderFinishedSemaphores; }
    std::vector<VkFence>& getInFlightFences() { return inFlightFences; }

private:
    std::vector<VkSemaphore> imageAvailableSemaphores;
    std::vector<VkSemaphore> renderFinishedSemaphores;
    std::vector<VkFence> inFlightFences;
};