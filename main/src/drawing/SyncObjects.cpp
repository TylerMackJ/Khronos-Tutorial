#include "SyncObjects.hpp"

#include "HelloTriangleApplication.hpp"

using App = HelloTriangleApplication;

SyncObjects::SyncObjects( Device& device ) : device( device )
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
        if( vkCreateSemaphore( device, &semaphoreInfo, nullptr, &imageAvailableSemaphores[i] ) != VK_SUCCESS ||
            vkCreateSemaphore( device, &semaphoreInfo, nullptr, &renderFinishedSemaphores[i] ) != VK_SUCCESS ||
            vkCreateFence( device, &fenceInfo, nullptr, &inFlightFences[i] ) != VK_SUCCESS )
        {
            throw std::runtime_error( "failed to create semaphores!" );
        }
    }
}

SyncObjects::~SyncObjects()
{
    for( size_t i = 0; i < App::get().MAX_FRAMES_IN_FLIGHT; i++ )
    {
        vkDestroySemaphore( device, imageAvailableSemaphores[i], nullptr );
        vkDestroySemaphore( device, renderFinishedSemaphores[i], nullptr );
        vkDestroyFence( device, inFlightFences[i], nullptr );
    }
}