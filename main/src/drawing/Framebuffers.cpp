#include "Framebuffers.hpp"

#include "HelloTriangleApplication.hpp"

#include <array>

using App = HelloTriangleApplication;

Framebuffers::Framebuffers( Device& device ) : device( device )
{
    swapchainFramebuffers.resize( App::get().getSwapchain().getSwapchainImages().size() );

    for( size_t i = 0; i < App::get().getSwapchain().getSwapchainImages().size(); i++ )
    {
        std::array< VkImageView, 3 > attachments = {
            App::get().getColorImage(), App::get().getDepthImage(), *( App::get().getImageViews()[i] )
        };

        VkFramebufferCreateInfo framebufferInfo{};
        framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        framebufferInfo.renderPass = App::get().getRenderPass();
        framebufferInfo.attachmentCount = static_cast< uint32_t >( attachments.size() );
        framebufferInfo.pAttachments = attachments.data();
        framebufferInfo.width = App::get().getSwapchain().width();
        framebufferInfo.height = App::get().getSwapchain().height();
        framebufferInfo.layers = 1;

        if( vkCreateFramebuffer( device, &framebufferInfo, nullptr, &swapchainFramebuffers[i] ) != VK_SUCCESS )
        {
            throw std::runtime_error( "failed to create framebuffer!" );
        }
    }
}

Framebuffers::~Framebuffers()
{
    for( auto framebuffer : swapchainFramebuffers )
    {
        vkDestroyFramebuffer( device, framebuffer, nullptr );
    }
}