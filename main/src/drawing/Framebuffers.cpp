#include "Framebuffers.hpp"

#include "HelloTriangleApplication.hpp"

#include <array>

using App = HelloTriangleApplication;

Framebuffers::Framebuffers( Device& device ) : device( device )
{
    swapChainFramebuffers.resize( App::get().getSwapChain().getSwapChainImages().size() );

    for( size_t i = 0; i < App::get().getSwapChain().getSwapChainImages().size(); i++ )
    {
        std::array< VkImageView, 3 > attachments = {
            App::get().getColorImage().getImageView(),
            App::get().getDepthImage().getImageView(),
            App::get().getImageViews()[i]->getImageView()
        };

        VkFramebufferCreateInfo framebufferInfo{};
        framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        framebufferInfo.renderPass = App::get().getRenderPass().getRenderPass();
        framebufferInfo.attachmentCount = static_cast< uint32_t >( attachments.size() );
        framebufferInfo.pAttachments = attachments.data();
        framebufferInfo.width = App::get().getSwapChain().getSwapChainExtent().width;
        framebufferInfo.height = App::get().getSwapChain().getSwapChainExtent().height;
        framebufferInfo.layers = 1;

        if( vkCreateFramebuffer( device, &framebufferInfo, nullptr, &swapChainFramebuffers[i] ) != VK_SUCCESS )
        {
            throw std::runtime_error( "failed to create framebuffer!" );
        }
    }
}

Framebuffers::~Framebuffers()
{
    for( auto framebuffer : swapChainFramebuffers )
    {
        vkDestroyFramebuffer( device, framebuffer, nullptr );
    }
}