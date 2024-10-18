#pragma once

#include <vector>

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
	static std::unique_ptr<GLFWInit> glfw;
	static std::unique_ptr<Window> window;
	static std::unique_ptr<Instance> instance;
	static std::unique_ptr<DebugMessenger> debugMessenger;
	static std::unique_ptr<Surface> surface;
	static std::unique_ptr<PhysicalDevice> physicalDevice;
	static std::unique_ptr<LogicalDevice> logicalDevice;
	static std::unique_ptr<SwapChain> swapChain;
	static std::unique_ptr<ImageViews> imageViews;
	static std::unique_ptr<RenderPass> renderPass;
	static std::unique_ptr<GraphicsPipeline> graphicsPipeline;
	static std::unique_ptr<Framebuffers> framebuffers;
	static std::unique_ptr<CommandPool> commandPool;
	static std::unique_ptr<CommandBuffer> commandBuffer;
	static std::unique_ptr<SyncObjects> syncObjects;

public:
	static const uint32_t MAX_FRAMES_IN_FLIGHT = 2;
	static const uint32_t WIDTH = 800;
	static const uint32_t HEIGHT = 600;

	static inline const std::vector<const char*> validationLayers = {
		"VK_LAYER_KHRONOS_validation"
	};
	
	static inline const std::vector<const char*> deviceExtensions = {
		VK_KHR_SWAPCHAIN_EXTENSION_NAME
	};

#ifdef NDEBUG
	static const bool enableValidationLayers = false;
#else
	static const bool enableValidationLayers = true;
#endif // NDEBUG

	static void run();

	static std::vector<char> readFile( const std::string& filename );

	static GLFWInit& getGLFWInit() { return *glfw; }
	static Window& getWindow() { return *window; }
	static Instance& getInstance() { return *instance; }
	static DebugMessenger& getDebugMessenger() { return *debugMessenger; }
	static Surface& getSurface() { return *surface; }
	static PhysicalDevice& getPhysicalDevice() { return *physicalDevice; }
	static LogicalDevice& getLogicalDevice() { return *logicalDevice; }
	static SwapChain& getSwapChain() { return *swapChain; }
	static ImageViews& getImageViews() { return *imageViews; }
	static RenderPass& getRenderPass() { return *renderPass; }
	static GraphicsPipeline& getGraphicsPipeline() { return *graphicsPipeline; }
	static Framebuffers& getFramebuffers() { return *framebuffers; }
	static CommandPool& getCommandPool() { return *commandPool; }
	static CommandBuffer& getCommandBuffer() { return *commandBuffer; }
	static SyncObjects& getSyncObjects() { return *syncObjects; }

private:
	static void drawFrame();
	static void recreateSwapChain();

	static uint32_t currentFrame;
};

