#include "SyncObjects.hpp"

#include "HelloTriangleApplication.hpp"

using App = HelloTriangleApplication;

SyncObjects::SyncObjects()
{
    imageAvailableSemaphores.resize( App::get().MAX_FRAMES_IN_FLIGHT );
    renderFinishedSemaphores.resize( App::get().MAX_FRAMES_IN_FLIGHT );
    inFlightFences.resize( App::get().MAX_FRAMES_IN_FLIGHT );

    VkSemaphoreCreateInfo semaphoreInfo = {};
    semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

    VkFenceCreateInfo fenceInfo = {};
    fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

    for( size_t i = 0; i < App::get().MAX_FRAMES_IN_FLIGHT; i++ )
    {
        if( vkCreateSemaphore(
                App::get().getLogicalDevice().getDeviceRef(), &semaphoreInfo, nullptr, &imageAvailableSemaphores[i]
            ) != VK_SUCCESS ||
            vkCreateSemaphore(
                App::get().getLogicalDevice().getDeviceRef(), &semaphoreInfo, nullptr, &renderFinishedSemaphores[i]
            ) != VK_SUCCESS ||
            vkCreateFence( App::get().getLogicalDevice().getDeviceRef(), &fenceInfo, nullptr, &inFlightFences[i] ) !=
                VK_SUCCESS )
        {
            throw std::runtime_error( "failed to create semaphores!" );
        }
    }
}

SyncObjects::~SyncObjects()
{
    for( size_t i = 0; i < App::get().MAX_FRAMES_IN_FLIGHT; i++ )
    {
        vkDestroySemaphore( App::get().getLogicalDevice().getDeviceRef(), imageAvailableSemaphores[i], nullptr );
        vkDestroySemaphore( App::get().getLogicalDevice().getDeviceRef(), renderFinishedSemaphores[i], nullptr );
        vkDestroyFence( App::get().getLogicalDevice().getDeviceRef(), inFlightFences[i], nullptr );
    }
}