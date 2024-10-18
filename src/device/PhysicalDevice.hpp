#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <optional>
#include <stdexcept>

#include "setup/Instance.hpp"
#include "presentation/Surface.hpp"

class PhysicalDevice
{
public:
	struct QueueFamilyIndices
	{
		std::optional<uint32_t> graphicsFamily;
		std::optional<uint32_t> presentFamily;

		bool isComplete()
		{
			return graphicsFamily.has_value() && presentFamily.has_value();
		}
	};
    struct SwapChainSupportDetails
    {
        VkSurfaceCapabilitiesKHR capabilities;
        std::vector<VkSurfaceFormatKHR> formats;
        std::vector<VkPresentModeKHR> presentModes;
    };


	PhysicalDevice();

	VkPhysicalDevice& getPhysicalDeviceRef() { return physicalDevice; }
	QueueFamilyIndices getQueueFamilyIndices();
	SwapChainSupportDetails getSwapChainSupportDetails();

private:
	int rateDeviceSuitability( VkPhysicalDevice device );
	QueueFamilyIndices findQueueFamilies( VkPhysicalDevice device );
	bool checkDeviceExtensionSupport( VkPhysicalDevice device );
    SwapChainSupportDetails querySwapChainSupport( VkPhysicalDevice device );

	VkPhysicalDevice physicalDevice;
};

