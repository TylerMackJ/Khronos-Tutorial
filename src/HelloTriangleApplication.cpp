#include "HelloTriangleApplication.hpp"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <fstream>
#include <iostream>

HelloTriangleApplication* HelloTriangleApplication::singletonInstance = nullptr;

void HelloTriangleApplication::init()
{
	glfw = std::make_unique<GLFWInit>();
	window = std::make_unique<Window>();
	instance = std::make_unique<Instance>();
	debugMessenger = std::make_unique<DebugMessenger>();
	surface = std::make_unique<Surface>();
	physicalDevice = std::make_unique<PhysicalDevice>();
	logicalDevice = std::make_unique<LogicalDevice>();
	swapChain = std::make_unique<SwapChain>();
	imageViews = std::make_unique<ImageViews>();
	renderPass = std::make_unique<RenderPass>();
	graphicsPipeline = std::make_unique<GraphicsPipeline>();
	framebuffers = std::make_unique<Framebuffers>();
	commandPool = std::make_unique<CommandPool>();
	commandBuffer = std::make_unique<CommandBuffer>();
	syncObjects = std::make_unique<SyncObjects>();
	currentFrame = 0;
}

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
	vkWaitForFences( getLogicalDevice().getDeviceRef(), 1, &(getSyncObjects().getInFlightFences()[currentFrame]), VK_TRUE, UINT64_MAX );

	uint32_t imageIndex;
	VkResult result = vkAcquireNextImageKHR( getLogicalDevice().getDeviceRef(), getSwapChain().getSwapChainRef(), UINT64_MAX, getSyncObjects().getImageAvailableSemaphores()[currentFrame], VK_NULL_HANDLE, &imageIndex );

	if( result == VK_ERROR_OUT_OF_DATE_KHR )
	{
		recreateSwapChain();
		return;
	}
	else if ( result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR )
	{
		throw std::runtime_error( "failed to acquire swap chain image!" );
	}

	vkResetFences( getLogicalDevice().getDeviceRef(), 1, &getSyncObjects().getInFlightFences()[currentFrame] );

	vkResetCommandBuffer( getCommandBuffer().getCommandBuffers()[currentFrame], 0 );

	getCommandBuffer().recordCommandBuffer( getCommandBuffer().getCommandBuffers()[currentFrame], imageIndex );

	VkSubmitInfo submitInfo = {};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	VkSemaphore waitSemaphores[] = { getSyncObjects().getImageAvailableSemaphores()[currentFrame] };
	VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
	submitInfo.waitSemaphoreCount = 1;
	submitInfo.pWaitSemaphores = waitSemaphores;
	submitInfo.pWaitDstStageMask = waitStages;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &getCommandBuffer().getCommandBuffers()[currentFrame];
	VkSemaphore signalSemaphores[] = { getSyncObjects().getRenderFinishedSemaphores()[currentFrame] };
	submitInfo.signalSemaphoreCount = 1;
	submitInfo.pSignalSemaphores = signalSemaphores;

	if( vkQueueSubmit( getLogicalDevice().getGraphicsQueueRef(), 1, &submitInfo, getSyncObjects().getInFlightFences()[currentFrame] ) != VK_SUCCESS )
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

	result = vkQueuePresentKHR( getLogicalDevice().getPresentQueueRef(), &presentInfo );

	if( result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || window->getFramebufferResized() )
	{
		window->setFramebufferResized( false );
		recreateSwapChain();
	}
	else if ( result != VK_SUCCESS )
	{
		throw std::runtime_error( "failed to present swap chain image!" );
	}

	currentFrame = ( currentFrame + 1 ) % MAX_FRAMES_IN_FLIGHT;
}

void HelloTriangleApplication::recreateSwapChain()
{
	int width = 0, height = 0;
	glfwGetFramebufferSize( window->get(), &width, &height );
	while( width == 0 || height == 0 )
	{
		glfwGetFramebufferSize( window->get(), &width, &height );
		glfwWaitEvents();
	}

	vkDeviceWaitIdle( getLogicalDevice().getDeviceRef() );

	framebuffers.reset();
	imageViews.reset();
	swapChain.reset();

	swapChain = std::make_unique<SwapChain>();
	imageViews = std::make_unique<ImageViews>();
	framebuffers = std::make_unique<Framebuffers>();
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