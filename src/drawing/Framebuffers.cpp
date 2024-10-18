#include "Framebuffers.hpp"

#include "HelloTriangleApplication.hpp"

using App = HelloTriangleApplication;

Framebuffers::Framebuffers() 
{
    swapChainFramebuffers.resize(App::get().getSwapChain().getSwapChainImages().size());

    for( size_t i = 0; i < App::get().getSwapChain().getSwapChainImages().size(); i++ )
    {
        VkImageView attachments[] =
        {
            App::get().getImageViews().getImageViews()[i]
        };

        VkFramebufferCreateInfo framebufferInfo{};
        framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        framebufferInfo.renderPass = App::get().getRenderPass().getRenderPass();
        framebufferInfo.attachmentCount = 1;
        framebufferInfo.pAttachments = attachments;
        framebufferInfo.width = App::get().getSwapChain().getSwapChainExtent().width;
        framebufferInfo.height = App::get().getSwapChain().getSwapChainExtent().height;
        framebufferInfo.layers = 1;

        if( vkCreateFramebuffer( App::get().getLogicalDevice().getDeviceRef(), &framebufferInfo, nullptr, &swapChainFramebuffers[i] ) != VK_SUCCESS )
        {
            throw std::runtime_error( "failed to create framebuffer!" );
        }
    }
}

Framebuffers::~Framebuffers() 
{
    for( auto framebuffer : swapChainFramebuffers )
    {
        vkDestroyFramebuffer( App::get().getLogicalDevice().getDeviceRef(), framebuffer, nullptr );
    }
}