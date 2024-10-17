#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

class LogicalDevice
{
public:
	LogicalDevice();
	~LogicalDevice();

	VkDevice& getDeviceRef() { return device; };
	VkQueue& getGraphicsQueueRef() { return graphicsQueue; }
	VkQueue& getPresentQueueRef() { return presentQueue; };

private:

	VkDevice device;
	VkQueue graphicsQueue;
	VkQueue presentQueue;
};

