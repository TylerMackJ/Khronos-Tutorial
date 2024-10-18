#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

class Window {
public:
	Window();

	~Window();

	GLFWwindow* get() const;
	const bool getFramebufferResized() const { return framebufferResized; }
	void setFramebufferResized( bool framebufferResized ) { this->framebufferResized = framebufferResized; } 

	static void framebufferResizeCallback( GLFWwindow* window, int width, int height );

private:
	GLFWwindow* window;
	bool framebufferResized;
};