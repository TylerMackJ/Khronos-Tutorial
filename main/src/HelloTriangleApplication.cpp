#include "HelloTriangleApplication.hpp"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#define GLM_FORCE_DEFAULT_ALIGNED_GENTYPES
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <chrono>
#include <fstream>
#include <iostream>

HelloTriangleApplication* HelloTriangleApplication::singletonInstance = nullptr;

void HelloTriangleApplication::constructor()
{
    window = std::make_unique< Window >( WIDTH, HEIGHT );
    device = std::make_unique< Device >(
        *window,
        deviceExtensions,
#ifndef NDEBUG
        std::make_optional< std::vector< const char* > >( validationLayers )
#endif
    );
    swapchain = std::make_unique< Swapchain >( *window, *device );
    createImageViews();
    textureSampler = std::make_unique< TextureSampler >( *device );
    renderPass = std::make_unique< RenderPass >( *device );
    descriptorSetLayout = std::make_unique< DescriptorSetLayout >( *device );
    graphicsPipeline = std::make_unique< GraphicsPipeline >( *device );
    commandPool = std::make_unique< CommandPool >( *device );
    colorImage = std::make_unique< Image >(
        *device,
        getSwapchain().width(),
        getSwapchain().height(),
        1,
        device->getMSAASamples(),
        getSwapchain(),
        VK_IMAGE_TILING_OPTIMAL,
        VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT | VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
        VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
    );
    colorImage->createImageView( VK_IMAGE_ASPECT_COLOR_BIT );
    depthImage = std::make_unique< Image >(
        *device,
        getSwapchain().width(),
        getSwapchain().height(),
        1,
        device->getMSAASamples(),
        findDepthFormat(),
        VK_IMAGE_TILING_OPTIMAL,
        VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
        VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
    );
    depthImage->createImageView( VK_IMAGE_ASPECT_DEPTH_BIT );
    depthImage->transitionLayout( VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL );
    framebuffers = std::make_unique< Framebuffers >( *device );
    textureImage = std::make_unique< Image >(
        *device,
        "textures/viking_room.png",
        VK_SAMPLE_COUNT_1_BIT,
        VK_FORMAT_R8G8B8A8_SRGB,
        VK_IMAGE_TILING_OPTIMAL,
        VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
        VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
    );
    textureImage->createImageView( VK_IMAGE_ASPECT_COLOR_BIT );
    vikingRoomModel = std::make_unique< ModelLoader::Model >( "models/viking_room.obj" );
    createVertexBuffer();
    createIndexBuffer();
    createUniformBuffers();
    descriptorPool = std::make_unique< DescriptorPool >( *device, static_cast< uint32_t >( MAX_FRAMES_IN_FLIGHT ) );
    createDescriptorSets();
    commandBuffer = std::make_unique< CommandBuffer >( *device, *commandPool, MAX_FRAMES_IN_FLIGHT );
    syncObjects = std::make_unique< SyncObjects >( *device );
    currentFrame = 0;
}

void HelloTriangleApplication::run()
{
    while( !glfwWindowShouldClose( *window ) )
    {
        glfwPollEvents();
        drawFrame();
    }

    vkDeviceWaitIdle( *device );
}

void HelloTriangleApplication::drawFrame()
{
    vkWaitForFences( *device, 1, &( getSyncObjects().getInFlightFences()[currentFrame] ), VK_TRUE, UINT64_MAX );

    uint32_t imageIndex;
    VkResult result = vkAcquireNextImageKHR(
        *device,
        getSwapchain(),
        UINT64_MAX,
        getSyncObjects().getImageAvailableSemaphores()[currentFrame],
        VK_NULL_HANDLE,
        &imageIndex
    );

    if( result == VK_ERROR_OUT_OF_DATE_KHR )
    {
        recreateSwapchain();
        return;
    }
    else if( result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR )
    {
        throw std::runtime_error( "failed to acquire swap chain image!" );
    }

    vkResetFences( *device, 1, &getSyncObjects().getInFlightFences()[currentFrame] );

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

    if( vkQueueSubmit(
            device->getGraphicsQueue(), 1, &submitInfo, getSyncObjects().getInFlightFences()[currentFrame]
        ) != VK_SUCCESS )
    {
        throw std::runtime_error( "failed to submit draw command buffer!" );
    }

    VkPresentInfoKHR presentInfo = {};
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores = signalSemaphores;
    VkSwapchainKHR swapchains[] = { getSwapchain() };
    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = swapchains;
    presentInfo.pImageIndices = &imageIndex;
    presentInfo.pResults = nullptr;

    result = vkQueuePresentKHR( device->getPresentQueue(), &presentInfo );

    if( result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || window->getFramebufferResized() )
    {
        window->setFramebufferResized( false );
        recreateSwapchain();
    }
    else if( result != VK_SUCCESS )
    {
        throw std::runtime_error( "failed to present swap chain image!" );
    }

    currentFrame = ( currentFrame + 1 ) % MAX_FRAMES_IN_FLIGHT;
}

void HelloTriangleApplication::recreateSwapchain()
{
    int width = 0, height = 0;
    glfwGetFramebufferSize( *window, &width, &height );
    while( width == 0 || height == 0 )
    {
        glfwGetFramebufferSize( *window, &width, &height );
        glfwWaitEvents();
    }

    vkDeviceWaitIdle( *device );

    framebuffers.reset();
    colorImage.reset();
    depthImage.reset();
    for( auto& imageView : imageViews )
    {
        imageView.reset();
    }
    swapchain.reset();

    swapchain = std::make_unique< Swapchain >( *window, *device );
    createImageViews();
    colorImage = std::make_unique< Image >(
        *device,
        getSwapchain().width(),
        getSwapchain().height(),
        1,
        device->getMSAASamples(),
        getSwapchain(),
        VK_IMAGE_TILING_OPTIMAL,
        VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT | VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
        VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
    );
    colorImage->createImageView( VK_IMAGE_ASPECT_COLOR_BIT );
    depthImage = std::make_unique< Image >(
        *device,
        getSwapchain().width(),
        getSwapchain().height(),
        1,
        device->getMSAASamples(),
        findDepthFormat(),
        VK_IMAGE_TILING_OPTIMAL,
        VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
        VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
    );
    depthImage->createImageView( VK_IMAGE_ASPECT_DEPTH_BIT );
    depthImage->transitionLayout( VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL );
    framebuffers = std::make_unique< Framebuffers >( *device );
}

void HelloTriangleApplication::updateUniformBuffer( uint32_t currentImage )
{
    static auto startTime = std::chrono::high_resolution_clock::now();

    auto currentTime = std::chrono::high_resolution_clock::now();
    float time = std::chrono::duration< float, std::chrono::seconds::period >( currentTime - startTime ).count();

    UniformBufferObject ubo = {};
    ubo.model = glm::rotate( glm::mat4( 1.0f ), time * glm::radians( 90.0f ), glm::vec3( 0.0f, 0.0f, 1.0f ) );
    ubo.view =
        glm::lookAt( glm::vec3( 2.0f, 2.0f, 2.0f ), glm::vec3( 0.0f, 0.0f, 0.0f ), glm::vec3( 0.0f, 0.0f, 1.0f ) );
    ubo.proj = glm::perspective(
        glm::radians( 45.0f ), getSwapchain().width() / ( float )getSwapchain().height(), 0.1f, 10.0f
    );
    ubo.proj[1][1] *= -1;

    uniformBuffers[currentImage]->copyTo( &ubo );
}

VkFormat HelloTriangleApplication::findDepthFormat()
{
    return device->findSupportedFormat(
        { VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT },
        VK_IMAGE_TILING_OPTIMAL,
        VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT
    );
}

void HelloTriangleApplication::createImageViews()
{
    imageViews.resize( getSwapchain().getSwapchainImages().size() );
    for( size_t i = 0; i < getSwapchain().getSwapchainImages().size(); i++ )
    {
        imageViews[i] = std::make_unique< ImageView >(
            *device, getSwapchain().getSwapchainImages()[i], getSwapchain(), VK_IMAGE_ASPECT_COLOR_BIT, 1
        );
    }
}

void HelloTriangleApplication::createVertexBuffer()
{
    VkDeviceSize bufferSize = sizeof( vikingRoomModel->getVertices()[0] ) * vikingRoomModel->getVertices().size();
    std::unique_ptr< Buffer > stagingBuffer = std::make_unique< Buffer >(
        *device,
        bufferSize,
        VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
    );

    void* data;
    vkMapMemory( *device, *stagingBuffer, 0, bufferSize, 0, &data );
    memcpy( data, vikingRoomModel->getVertices().data(), ( size_t )bufferSize );
    vkUnmapMemory( *device, *stagingBuffer );

    vertexBuffer = std::make_unique< Buffer >(
        *device,
        bufferSize,
        VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
        VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
    );

    Buffer::copyBuffer( *device, *stagingBuffer, *vertexBuffer, bufferSize );
}

void HelloTriangleApplication::createIndexBuffer()
{
    VkDeviceSize bufferSize = sizeof( vikingRoomModel->getIndices()[0] ) * vikingRoomModel->getIndices().size();

    std::unique_ptr< Buffer > stagingBuffer = std::make_unique< Buffer >(
        *device,
        bufferSize,
        VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
    );

    stagingBuffer->mapMemory( 0, bufferSize, 0 );
    stagingBuffer->copyTo( vikingRoomModel->getIndices().data() );
    stagingBuffer->unmapMemory();

    indexBuffer = std::make_unique< Buffer >(
        *device,
        bufferSize,
        VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
        VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
    );

    Buffer::copyBuffer( *device, *stagingBuffer, *indexBuffer, bufferSize );
}

void HelloTriangleApplication::createUniformBuffers()
{
    VkDeviceSize bufferSize = sizeof( UniformBufferObject );

    uniformBuffers.resize( MAX_FRAMES_IN_FLIGHT );

    for( size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++ )
    {
        uniformBuffers[i] = std::make_unique< Buffer >(
            *device,
            bufferSize,
            VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
        );

        uniformBuffers[i]->mapMemory( 0, bufferSize, 0 );
    }
}

void HelloTriangleApplication::createDescriptorSets()
{
    std::vector< VkDescriptorSetLayout > layouts( MAX_FRAMES_IN_FLIGHT, *descriptorSetLayout );
    VkDescriptorSetAllocateInfo allocInfo = {};
    allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    allocInfo.descriptorPool = *descriptorPool;
    allocInfo.descriptorSetCount = static_cast< uint32_t >( MAX_FRAMES_IN_FLIGHT );
    allocInfo.pSetLayouts = layouts.data();

    descriptorSets.resize( MAX_FRAMES_IN_FLIGHT );
    if( vkAllocateDescriptorSets( *device, &allocInfo, descriptorSets.data() ) != VK_SUCCESS )
    {
        throw std::runtime_error( "failed to allocate descriptor sets!" );
    }

    for( size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++ )
    {
        VkDescriptorBufferInfo bufferInfo{};
        bufferInfo.buffer = *( uniformBuffers[i] );
        bufferInfo.offset = 0;
        bufferInfo.range = sizeof( UniformBufferObject );

        VkDescriptorImageInfo imageInfo{};
        imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
        imageInfo.imageView = *textureImage;
        imageInfo.sampler = *textureSampler;

        std::array< VkWriteDescriptorSet, 2 > descriptorWrites{};

        descriptorWrites[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        descriptorWrites[0].dstSet = descriptorSets[i];
        descriptorWrites[0].dstBinding = 0;
        descriptorWrites[0].dstArrayElement = 0;
        descriptorWrites[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        descriptorWrites[0].descriptorCount = 1;
        descriptorWrites[0].pBufferInfo = &bufferInfo;

        descriptorWrites[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        descriptorWrites[1].dstSet = descriptorSets[i];
        descriptorWrites[1].dstBinding = 1;
        descriptorWrites[1].dstArrayElement = 0;
        descriptorWrites[1].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        descriptorWrites[1].descriptorCount = 1;
        descriptorWrites[1].pImageInfo = &imageInfo;

        vkUpdateDescriptorSets(
            *device, static_cast< uint32_t >( descriptorWrites.size() ), descriptorWrites.data(), 0, nullptr
        );
    }
}

std::vector< char > HelloTriangleApplication::readFile( const std::string& filename )
{
    std::ifstream file( filename, std::ios::ate | std::ios::binary );

    if( !file.is_open() )
    {
        throw std::runtime_error( "failed to open file: " + filename + "!" );
    }

    size_t fileSize = ( size_t )file.tellg();
    std::vector< char > buffer( fileSize );

    file.seekg( 0 );
    file.read( buffer.data(), fileSize );

    file.close();

    return buffer;
}