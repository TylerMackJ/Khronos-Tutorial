#pragma once

#include <vector>
#include <iostream>

#include "device/LogicalDevice.hpp"
#include "device/PhysicalDevice.hpp"
#include "drawing/CommandBuffer.hpp"
#include "drawing/CommandPool.hpp"
#include "drawing/Framebuffers.hpp"
#include "drawing/SyncObjects.hpp"
#include "graphicsPipeline/GraphicsPipeline.hpp"
#include "graphicsPipeline/RenderPass.hpp"
#include "graphicsPipeline/Vertex.hpp"
#include "graphicsPipeline/Buffer.hpp"
#include "presentation/ImageViews.hpp"
#include "presentation/Surface.hpp"
#include "presentation/SwapChain.hpp"
#include "setup/DebugMessenger.hpp"
#include "setup/GLFWInit.hpp"
#include "setup/Instance.hpp"
#include "setup/Window.hpp"

class HelloTriangleApplication
{
private:	
	static HelloTriangleApplication* singletonInstance;

	HelloTriangleApplication() {}

public:
	HelloTriangleApplication(const HelloTriangleApplication&) = delete;
	HelloTriangleApplication& operator=(const HelloTriangleApplication&) = delete;

	static HelloTriangleApplication& get()
	{
		if (singletonInstance == nullptr)
		{
			singletonInstance = new HelloTriangleApplication();
		}

		return *singletonInstance;
	}

	void init();

	void run();

private:
	std::unique_ptr<GLFWInit> glfw;
	std::unique_ptr<Window> window;
	std::unique_ptr<Instance> instance;
	std::unique_ptr<DebugMessenger> debugMessenger;
	std::unique_ptr<Surface> surface;
	std::unique_ptr<PhysicalDevice> physicalDevice;
	std::unique_ptr<LogicalDevice> logicalDevice;
	std::unique_ptr<SwapChain> swapChain;
	std::unique_ptr<ImageViews> imageViews;
	std::unique_ptr<RenderPass> renderPass;
	std::unique_ptr<GraphicsPipeline> graphicsPipeline;
	std::unique_ptr<Framebuffers> framebuffers;
	std::unique_ptr<CommandPool> commandPool;
	std::unique_ptr<Buffer> vertexBuffer;
	std::unique_ptr<Buffer> indexBuffer;
	std::unique_ptr<CommandBuffer> commandBuffer;
	std::unique_ptr<SyncObjects> syncObjects;

public:
	CommandBuffer& getCommandBuffer() { return *commandBuffer; }
	CommandPool& getCommandPool() { return *commandPool; }
	DebugMessenger& getDebugMessenger() { return *debugMessenger; }
	Framebuffers& getFramebuffers() { return *framebuffers; }
	GLFWInit& getGLFWInit() { return *glfw; }
	GraphicsPipeline& getGraphicsPipeline() { return *graphicsPipeline; }
	ImageViews& getImageViews() { return *imageViews; }
	Instance& getInstance() { return *instance; }
	LogicalDevice& getLogicalDevice() { return *logicalDevice; }
	PhysicalDevice& getPhysicalDevice() { return *physicalDevice; }
	RenderPass& getRenderPass() { return *renderPass; }
	Surface& getSurface() { return *surface; }
	SwapChain& getSwapChain() { return *swapChain; }
	SyncObjects& getSyncObjects() { return *syncObjects; }
	Buffer& getVertexBuffer() { return *vertexBuffer; }
	Buffer& getIndexBuffer() { return *indexBuffer; }
	Window& getWindow() { return *window; }

public:
	const uint32_t MAX_FRAMES_IN_FLIGHT = 2;
	const uint32_t WIDTH = 800;
	const uint32_t HEIGHT = 600;

	const std::vector<const char*> validationLayers = {
		"VK_LAYER_KHRONOS_validation"
	};
	
	const std::vector<const char*> deviceExtensions = {
		VK_KHR_SWAPCHAIN_EXTENSION_NAME
	};

#ifdef NDEBUG
	const bool enableValidationLayers = false;
#else
	const bool enableValidationLayers = true;
#endif // NDEBUG

	const std::vector<Vertex> vertices = {
		{ {-0.5f, -0.5f}, {1.0f, 0.0f, 0.0f} },
		{ { 0.5f, -0.5f}, {0.0f, 1.0f, 0.0f} },
		{ { 0.5f,  0.5f}, {0.0f, 0.0f, 1.0f} },
		{ {-0.5f,  0.5f}, {1.0f, 1.0f, 1.0f} }
	};

	const std::vector<uint16_t> indices = {
		0, 1, 2,
		2, 3, 0
	};

	static std::vector<char> readFile( const std::string& filename );

private:
	void drawFrame();
	void recreateSwapChain();

	void createVertexBuffer();
	void createIndexBuffer();

	uint32_t currentFrame;
};

