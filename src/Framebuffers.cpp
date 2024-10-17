#include "Framebuffers.hpp"

#include "HelloTriangleApplication.hpp"

Framebuffers::Framebuffers() 
{
    swapChainFramebuffers.resize(HelloTriangleApplication::getSwapChain().getSwapChainImages().size());

    for( size_t i = 0; i < swapChainFramebuffers.size(); i++ )
    {
        VkImageView attachments[] =
        {
            HelloTriangleApplication::getImageViews().getImageViews()[i]
        };

        VkFramebufferCreateInfo framebufferInfo{};
        framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        framebufferInfo.renderPass = HelloTriangleApplication::getRenderPass().getRenderPass();
        framebufferInfo.attachmentCount = 1;
        framebufferInfo.pAttachments = attachments;
        framebufferInfo.width = HelloTriangleApplication::getSwapChain().getSwapChainExtent().width;
        framebufferInfo.height = HelloTriangleApplication::getSwapChain().getSwapChainExtent().height;
        framebufferInfo.layers = 1;

        if( vkCreateFramebuffer( HelloTriangleApplication::getLogicalDevice().getDeviceRef(), &framebufferInfo, nullptr, &swapChainFramebuffers[i] ) != VK_SUCCESS )
        {
            throw std::runtime_error( "failed to create framebuffer!" );
        }
    }
}

Framebuffers::~Framebuffers() 
{
    for( auto framebuffer : swapChainFramebuffers )
    {
        vkDestroyFramebuffer( HelloTriangleApplication::getLogicalDevice().getDeviceRef(), framebuffer, nullptr );
    }
}