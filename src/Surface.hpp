#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "Instance.hpp"
#include "Window.hpp"

class Surface
{
public:
	Surface();
	~Surface();

	VkSurfaceKHR& getSurfaceRef() { return surface; };

private:
	VkSurfaceKHR surface;
};

