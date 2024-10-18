#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

class Window {
public:
	Window();

	~Window();

	GLFWwindow* get() const;

private:
	GLFWwindow* window;
};