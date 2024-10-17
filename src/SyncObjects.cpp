#include "SyncObjects.hpp"

#include "HelloTriangleApplication.hpp"

SyncObjects::SyncObjects()
{
    VkSemaphoreCreateInfo semaphoreInfo = {};
    semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

    VkFenceCreateInfo fenceInfo = {};
    fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

    if (vkCreateSemaphore(HelloTriangleApplication::getLogicalDevice().getDeviceRef(), &semaphoreInfo, nullptr, &imageAvailableSemaphore) != VK_SUCCESS ||
        vkCreateSemaphore(HelloTriangleApplication::getLogicalDevice().getDeviceRef(), &semaphoreInfo, nullptr, &renderFinishedSemaphore) != VK_SUCCESS ||
        vkCreateFence(HelloTriangleApplication::getLogicalDevice().getDeviceRef(), &fenceInfo, nullptr, &inFlightFence) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create semaphores!");
    }
}

SyncObjects::~SyncObjects()
{
    vkDestroySemaphore(HelloTriangleApplication::getLogicalDevice().getDeviceRef(), imageAvailableSemaphore, nullptr);
    vkDestroySemaphore(HelloTriangleApplication::getLogicalDevice().getDeviceRef(), renderFinishedSemaphore, nullptr);
    vkDestroyFence(HelloTriangleApplication::getLogicalDevice().getDeviceRef(), inFlightFence, nullptr);
}