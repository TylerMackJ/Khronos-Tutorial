#include "Window.hpp"

#include <iostream>

#include "HelloTriangleApplication.hpp"

Window::Window()
{
	glfwWindowHint( GLFW_CLIENT_API, GLFW_NO_API );
	glfwWindowHint( GLFW_RESIZABLE, GLFW_FALSE );

	window = glfwCreateWindow( HelloTriangleApplication::WIDTH, HelloTriangleApplication::HEIGHT, "Vulkan", nullptr, nullptr );
}

Window::~Window()
{
	glfwDestroyWindow( window );
}

GLFWwindow* Window::get() const
{
	return window;
}