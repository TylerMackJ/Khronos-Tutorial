#include "SyncObjects.hpp"

#include "HelloTriangleApplication.hpp"

SyncObjects::SyncObjects()
{
    imageAvailableSemaphores.resize(HelloTriangleApplication::MAX_FRAMES_IN_FLIGHT);
    renderFinishedSemaphores.resize(HelloTriangleApplication::MAX_FRAMES_IN_FLIGHT);
    inFlightFences.resize(HelloTriangleApplication::MAX_FRAMES_IN_FLIGHT);

    VkSemaphoreCreateInfo semaphoreInfo = {};
    semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

    VkFenceCreateInfo fenceInfo = {};
    fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

    for (size_t i = 0; i < HelloTriangleApplication::MAX_FRAMES_IN_FLIGHT; i++)
    {
        if (vkCreateSemaphore(HelloTriangleApplication::getLogicalDevice().getDeviceRef(), &semaphoreInfo, nullptr, &imageAvailableSemaphores[i]) != VK_SUCCESS ||
            vkCreateSemaphore(HelloTriangleApplication::getLogicalDevice().getDeviceRef(), &semaphoreInfo, nullptr, &renderFinishedSemaphores[i]) != VK_SUCCESS ||
            vkCreateFence(HelloTriangleApplication::getLogicalDevice().getDeviceRef(), &fenceInfo, nullptr, &inFlightFences[i]) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to create semaphores!");
        }
    }
}

SyncObjects::~SyncObjects()
{
    for (size_t i = 0; i < HelloTriangleApplication::MAX_FRAMES_IN_FLIGHT; i++)
    {
        vkDestroySemaphore(HelloTriangleApplication::getLogicalDevice().getDeviceRef(), imageAvailableSemaphores[i], nullptr);
        vkDestroySemaphore(HelloTriangleApplication::getLogicalDevice().getDeviceRef(), renderFinishedSemaphores[i], nullptr);
        vkDestroyFence(HelloTriangleApplication::getLogicalDevice().getDeviceRef(), inFlightFences[i], nullptr);
    }
}