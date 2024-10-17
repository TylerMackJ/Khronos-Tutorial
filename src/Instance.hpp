#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <memory>
#include <vector>

class Instance
{
public:
	Instance();

	~Instance();

	VkInstance& getInstanceRef() { return instance; };

private:
	bool checkValidationLayerSupport();
	std::vector<const char*> getRequiredExtensions();
	void printExtensions();

	VkInstance instance;
};
