#pragma once

#include <vector>
#include <iostream>

#include "setup/DebugMessenger.hpp"
#include "setup/GLFWInit.hpp"
#include "setup/Instance.hpp"
#include "device/LogicalDevice.hpp"
#include "device/PhysicalDevice.hpp"
#include "presentation/Surface.hpp"
#include "setup/Window.hpp"
#include "presentation/SwapChain.hpp"
#include "presentation/ImageViews.hpp"
#include "graphicsPipeline/RenderPass.hpp"
#include "graphicsPipeline/GraphicsPipeline.hpp"
#include "drawing/Framebuffers.hpp"
#include "drawing/CommandPool.hpp"
#include "drawing/CommandBuffer.hpp"
#include "drawing/SyncObjects.hpp"

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
	std::unique_ptr<CommandBuffer> commandBuffer;
	std::unique_ptr<SyncObjects> syncObjects;

public:
	GLFWInit& getGLFWInit() { return *glfw; }
	Window& getWindow() { return *window; }
	Instance& getInstance() { return *instance; }
	DebugMessenger& getDebugMessenger() { return *debugMessenger; }
	Surface& getSurface() { return *surface; }
	PhysicalDevice& getPhysicalDevice() { return *physicalDevice; }
	LogicalDevice& getLogicalDevice() { return *logicalDevice; }
	SwapChain& getSwapChain() { return *swapChain; }
	ImageViews& getImageViews() { return *imageViews; }
	RenderPass& getRenderPass() { return *renderPass; }
	GraphicsPipeline& getGraphicsPipeline() { return *graphicsPipeline; }
	Framebuffers& getFramebuffers() { return *framebuffers; }
	CommandPool& getCommandPool() { return *commandPool; }
	CommandBuffer& getCommandBuffer() { return *commandBuffer; }
	SyncObjects& getSyncObjects() { return *syncObjects; }

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

	static std::vector<char> readFile( const std::string& filename );

private:
	void drawFrame();
	void recreateSwapChain();

	uint32_t currentFrame;
};

