#include "HelloTriangleApplication.hpp"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <fstream>
#include <iostream>

std::unique_ptr<GLFWInit> HelloTriangleApplication::glfw = std::make_unique<GLFWInit>();
std::unique_ptr<Window> HelloTriangleApplication::window = std::make_unique<Window>();
std::unique_ptr<Instance> HelloTriangleApplication::instance = std::make_unique<Instance>();
std::unique_ptr<DebugMessenger> HelloTriangleApplication::debugMessenger = std::make_unique<DebugMessenger>();
std::unique_ptr<Surface> HelloTriangleApplication::surface = std::make_unique<Surface>();
std::unique_ptr<PhysicalDevice> HelloTriangleApplication::physicalDevice = std::make_unique<PhysicalDevice>();
std::unique_ptr<LogicalDevice> HelloTriangleApplication::logicalDevice = std::make_unique<LogicalDevice>();
std::unique_ptr<SwapChain> HelloTriangleApplication::swapChain = std::make_unique<SwapChain>();
std::unique_ptr<ImageViews> HelloTriangleApplication::imageViews = std::make_unique<ImageViews>();
std::unique_ptr<RenderPass> HelloTriangleApplication::renderPass = std::make_unique<RenderPass>();
std::unique_ptr<GraphicsPipeline> HelloTriangleApplication::graphicsPipeline = std::make_unique<GraphicsPipeline>();
std::unique_ptr<Framebuffers> HelloTriangleApplication::framebuffers = std::make_unique<Framebuffers>();
std::unique_ptr<CommandPool> HelloTriangleApplication::commandPool = std::make_unique<CommandPool>();
std::unique_ptr<CommandBuffer> HelloTriangleApplication::commandBuffer = std::make_unique<CommandBuffer>();
std::unique_ptr<SyncObjects> HelloTriangleApplication::syncObjects = std::make_unique<SyncObjects>();

void HelloTriangleApplication::run()
{
	while( !glfwWindowShouldClose( window->get() ) )
	{
		glfwPollEvents();
		drawFrame();
	}
	
	vkDeviceWaitIdle( getLogicalDevice().getDeviceRef() );
}

void HelloTriangleApplication::drawFrame()
{
	vkWaitForFences( getLogicalDevice().getDeviceRef(), 1, &getSyncObjects().getInFlightFence(), VK_TRUE, UINT64_MAX );
	vkResetFences( getLogicalDevice().getDeviceRef(), 1, &getSyncObjects().getInFlightFence() );

	uint32_t imageIndex;
	vkAcquireNextImageKHR( getLogicalDevice().getDeviceRef(), getSwapChain().getSwapChainRef(), UINT64_MAX, getSyncObjects().getImageAvailableSemaphore(), VK_NULL_HANDLE, &imageIndex );

	vkResetCommandBuffer( getCommandBuffer().getCommandBuffer(), 0 );

	getCommandBuffer().recordCommandBuffer( getCommandBuffer().getCommandBuffer(), imageIndex );

	VkSubmitInfo submitInfo = {};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	VkSemaphore waitSemaphores[] = { getSyncObjects().getImageAvailableSemaphore() };
	VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
	submitInfo.waitSemaphoreCount = 1;
	submitInfo.pWaitSemaphores = waitSemaphores;
	submitInfo.pWaitDstStageMask = waitStages;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &getCommandBuffer().getCommandBuffer();
	VkSemaphore signalSemaphores[] = { getSyncObjects().getRenderFinishedSemaphore() };
	submitInfo.signalSemaphoreCount = 1;
	submitInfo.pSignalSemaphores = signalSemaphores;

	if( vkQueueSubmit( getLogicalDevice().getGraphicsQueueRef(), 1, &submitInfo, getSyncObjects().getInFlightFence() ) != VK_SUCCESS )
	{
		throw std::runtime_error( "failed to submit draw command buffer!" );
	}

	VkPresentInfoKHR presentInfo = {};
	presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
	presentInfo.waitSemaphoreCount = 1;
	presentInfo.pWaitSemaphores = signalSemaphores;
	VkSwapchainKHR swapChains[] = { getSwapChain().getSwapChainRef() };
	presentInfo.swapchainCount = 1;
	presentInfo.pSwapchains = swapChains;
	presentInfo.pImageIndices = &imageIndex;
	presentInfo.pResults = nullptr;

	vkQueuePresentKHR( getLogicalDevice().getPresentQueueRef(), &presentInfo );
}

std::vector<char> HelloTriangleApplication::readFile( const std::string& filename )
{
	std::ifstream file( filename, std::ios::ate | std::ios::binary );

	if( !file.is_open() )
	{
		throw std::runtime_error( "failed to open file!" );
	}

	size_t fileSize = ( size_t )file.tellg();
	std::vector<char> buffer( fileSize );

	file.seekg( 0 );
	file.read( buffer.data(), fileSize );

	file.close();

	return buffer;
}