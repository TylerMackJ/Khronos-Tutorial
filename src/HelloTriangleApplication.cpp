#include "HelloTriangleApplication.hpp"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#define GLM_FORCE_DEFAULT_ALIGNED_GENTYPES
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>

#include <chrono>
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
	descriptorSetLayout = std::make_unique<DescriptorSetLayout>();
	graphicsPipeline = std::make_unique<GraphicsPipeline>();
	framebuffers = std::make_unique<Framebuffers>();
	commandPool = std::make_unique<CommandPool>();
	createVertexBuffer();
	createIndexBuffer();
	createUniformBuffers();
	descriptorPool = std::make_unique<DescriptorPool>();
	createDescriptorSets();
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

	updateUniformBuffer( currentFrame );

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

void HelloTriangleApplication::updateUniformBuffer( uint32_t currentImage )
{
	static auto startTime = std::chrono::high_resolution_clock::now();

	auto currentTime = std::chrono::high_resolution_clock::now();
	float time = std::chrono::duration<float, std::chrono::seconds::period>( currentTime - startTime ).count();

	UniformBufferObject ubo = {};
	ubo.model = glm::rotate( glm::mat4( 1.0f ), time * glm::radians( 90.0f ), glm::vec3( 0.0f, 0.0f, 1.0f ) );
	ubo.view = glm::lookAt( glm::vec3( 2.0f, 2.0f, 2.0f ), glm::vec3( 0.0f, 0.0f, 0.0f ), glm::vec3( 0.0f, 0.0f, 1.0f ) );
	ubo.proj = glm::perspective( glm::radians( 45.0f ), getSwapChain().getSwapChainExtent().width / (float)getSwapChain().getSwapChainExtent().height, 0.1f, 10.0f );
	ubo.proj[1][1] *= -1;

	uniformBuffers[currentImage]->copyTo( &ubo );
}

void HelloTriangleApplication::createVertexBuffer()
{
	VkDeviceSize bufferSize = sizeof( vertices[0] ) * vertices.size();
	std::unique_ptr<Buffer> stagingBuffer = std::make_unique<Buffer>(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

	void* data;
	vkMapMemory( getLogicalDevice().getDeviceRef(), stagingBuffer->getBufferMemory(), 0, bufferSize, 0, &data );
	memcpy( data, vertices.data(), (size_t)bufferSize );
	vkUnmapMemory( getLogicalDevice().getDeviceRef(), stagingBuffer->getBufferMemory() );

	vertexBuffer = std::make_unique<Buffer>(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT );

	Buffer::copyBuffer( *stagingBuffer, *vertexBuffer, bufferSize );
}

void HelloTriangleApplication::createIndexBuffer()
{
	VkDeviceSize bufferSize = sizeof( indices[0] ) * indices.size();

	std::unique_ptr<Buffer> stagingBuffer = std::make_unique<Buffer>(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

	stagingBuffer->mapMemory( 0, bufferSize, 0 );
	stagingBuffer->copyTo( indices.data() );
	stagingBuffer->unmapMemory();

	indexBuffer = std::make_unique<Buffer>(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT );

	Buffer::copyBuffer( *stagingBuffer, *indexBuffer, bufferSize );
}

void HelloTriangleApplication::createUniformBuffers()
{
	VkDeviceSize bufferSize = sizeof( UniformBufferObject );

	uniformBuffers.resize( MAX_FRAMES_IN_FLIGHT );

	for( size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++ )
	{
		uniformBuffers[i] = std::make_unique<Buffer>( bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT );

		uniformBuffers[i]->mapMemory( 0, bufferSize, 0 );
	}
}

void HelloTriangleApplication::createDescriptorSets()
{
	std::vector<VkDescriptorSetLayout> layouts( MAX_FRAMES_IN_FLIGHT, descriptorSetLayout->getDescriptorSetLayout() );
	VkDescriptorSetAllocateInfo allocInfo = {};
	allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	allocInfo.descriptorPool = descriptorPool->getDescriptorPool();
	allocInfo.descriptorSetCount = static_cast<uint32_t>( MAX_FRAMES_IN_FLIGHT );
	allocInfo.pSetLayouts = layouts.data();

	descriptorSets.resize( MAX_FRAMES_IN_FLIGHT );
	if( vkAllocateDescriptorSets( getLogicalDevice().getDeviceRef(), &allocInfo, descriptorSets.data() ) != VK_SUCCESS )
	{
		throw std::runtime_error( "failed to allocate descriptor sets!" );
	}

	for( size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++ )
	{
		VkDescriptorBufferInfo bufferInfo = {};
		bufferInfo.buffer = uniformBuffers[i]->getBuffer();
		bufferInfo.offset = 0;
		bufferInfo.range = sizeof( UniformBufferObject );

		VkWriteDescriptorSet descriptorWrite = {};
		descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descriptorWrite.dstSet = descriptorSets[i];
		descriptorWrite.dstBinding = 0;
		descriptorWrite.dstArrayElement = 0;
		descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		descriptorWrite.descriptorCount = 1;
		descriptorWrite.pBufferInfo = &bufferInfo;

		vkUpdateDescriptorSets( getLogicalDevice().getDeviceRef(), 1, &descriptorWrite, 0, nullptr );
	}
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