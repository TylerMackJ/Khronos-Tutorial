#include "CommandBuffer.hpp"

#include "HelloTriangleApplication.hpp"

#include <array>

using App = HelloTriangleApplication;

CommandBuffer::CommandBuffer( Device& device, CommandPool& commandPool, uint32_t maxFramesInFlight ) : device( device )
{
    commandBuffers.resize( maxFramesInFlight );

    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.commandPool = commandPool;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandBufferCount = ( uint32_t )commandBuffers.size();

    if( vkAllocateCommandBuffers( device, &allocInfo, commandBuffers.data() ) != VK_SUCCESS )
    {
        throw std::runtime_error( "failed to allocate command buffers!" );
    }
}

CommandBuffer::~CommandBuffer() {}

void CommandBuffer::recordCommandBuffer( VkCommandBuffer commandBuffer, uint32_t imageIndex )
{
    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = 0;
    beginInfo.pInheritanceInfo = nullptr;

    if( vkBeginCommandBuffer( commandBuffer, &beginInfo ) != VK_SUCCESS )
    {
        throw std::runtime_error( "failed to begin recording command buffer!" );
    }

    std::array< VkClearValue, 2 > clearValues{};
    clearValues[0].color = { 0.0f, 0.0f, 0.0f, 1.0f };
    clearValues[1].depthStencil = { 1.0f, 0 };

    VkRenderPassBeginInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassInfo.renderPass = App::get().getRenderPass();
    renderPassInfo.framebuffer = App::get().getFramebuffers().getFramebuffers()[imageIndex];
    renderPassInfo.renderArea.offset = { 0, 0 };
    renderPassInfo.renderArea.extent = App::get().getSwapchain();
    renderPassInfo.clearValueCount = static_cast< uint32_t >( clearValues.size() );
    renderPassInfo.pClearValues = clearValues.data();

    vkCmdBeginRenderPass( commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE );

    vkCmdBindPipeline( commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, App::get().getGraphicsPipeline() );

    VkViewport viewport{};
    viewport.x = 0.0f;
    viewport.y = 0.0f;
    viewport.width = static_cast< float >( App::get().getSwapchain().width() );
    viewport.height = static_cast< float >( App::get().getSwapchain().height() );
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;
    vkCmdSetViewport( commandBuffer, 0, 1, &viewport );

    VkRect2D scissor{};
    scissor.offset = { 0, 0 };
    scissor.extent = App::get().getSwapchain();
    vkCmdSetScissor( commandBuffer, 0, 1, &scissor );

    VkBuffer vertexBuffers[] = { App::get().getVertexBuffer() };
    VkDeviceSize offsets[] = { 0 };
    vkCmdBindVertexBuffers( commandBuffer, 0, 1, vertexBuffers, offsets );

    vkCmdBindIndexBuffer( commandBuffer, App::get().getIndexBuffer(), 0, VK_INDEX_TYPE_UINT32 );

    vkCmdBindDescriptorSets(
        commandBuffer,
        VK_PIPELINE_BIND_POINT_GRAPHICS,
        App::get().getGraphicsPipeline(),
        0,
        1,
        &App::get().getDescriptorSets()[imageIndex],
        0,
        nullptr
    );
    vkCmdDrawIndexed(
        commandBuffer, static_cast< uint32_t >( App::get().getVikingRoomModel().getIndices().size() ), 1, 0, 0, 0
    );

    vkCmdEndRenderPass( commandBuffer );

    if( vkEndCommandBuffer( commandBuffer ) != VK_SUCCESS )
    {
        throw std::runtime_error( "failed to record command buffer!" );
    }
}

VkCommandBuffer CommandBuffer::beginSingleTimeCommands( Device& device )
{
    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandPool = App::get().getCommandPool();
    allocInfo.commandBufferCount = 1;

    VkCommandBuffer commandBuffer;
    vkAllocateCommandBuffers( device, &allocInfo, &commandBuffer );

    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

    vkBeginCommandBuffer( commandBuffer, &beginInfo );

    return commandBuffer;
}
void CommandBuffer::endSingleTimeCommands( Device& device, VkCommandBuffer commandBuffer )
{
    vkEndCommandBuffer( commandBuffer );

    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &commandBuffer;

    vkQueueSubmit( device.getGraphicsQueue(), 1, &submitInfo, VK_NULL_HANDLE );
    vkQueueWaitIdle( device.getGraphicsQueue() );

    vkFreeCommandBuffers( device, App::get().getCommandPool(), 1, &commandBuffer );
}