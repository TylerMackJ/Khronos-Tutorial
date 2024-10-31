#include "Window.hpp"

#include <iostream>

#include "HelloTriangleApplication.hpp"

using App = HelloTriangleApplication;

Window::Window() : framebufferResized( false )
{
    glfwWindowHint( GLFW_CLIENT_API, GLFW_NO_API );
    // glfwWindowHint( GLFW_RESIZABLE, GLFW_FALSE );

    window = glfwCreateWindow( App::get().WIDTH, App::get().HEIGHT, "Vulkan", nullptr, nullptr );
    glfwSetWindowUserPointer( window, this );
    glfwSetFramebufferSizeCallback( window, framebufferResizeCallback );
}

Window::~Window() { glfwDestroyWindow( window ); }

GLFWwindow* Window::get() const { return window; }

void Window::framebufferResizeCallback( GLFWwindow* window, int /*width*/, int /*height*/ )
{
    auto app = reinterpret_cast< Window* >( glfwGetWindowUserPointer( window ) );
    app->setFramebufferResized( true );
}