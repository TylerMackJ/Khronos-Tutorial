#include "Window.hpp"

#include <iostream>

#include "HelloTriangleApplication.hpp"

using App = HelloTriangleApplication;

Window::Window( int width, int height ) : framebufferResized( false )
{
    glfwInit();
    glfwWindowHint( GLFW_CLIENT_API, GLFW_NO_API );

    window = glfwCreateWindow( width, height, "Vulkan", nullptr, nullptr );
    glfwSetWindowUserPointer( window, this );
    glfwSetFramebufferSizeCallback( window, framebufferResizeCallback );

    instance = std::make_unique< Instance >();
    debugMessenger = std::make_unique< DebugMessenger >( *instance );

    if( glfwCreateWindowSurface( *instance, window, nullptr, &surface ) != VK_SUCCESS )
    {
        throw std::runtime_error( "failed to create window surface!" );
    }
}

Window::~Window()
{
    vkDestroySurfaceKHR( *instance, surface, nullptr );
    debugMessenger.reset();
    instance.reset();
    glfwDestroyWindow( window );
    glfwTerminate();
}

void Window::framebufferResizeCallback( GLFWwindow* window, int /*width*/, int /*height*/ )
{
    auto app = reinterpret_cast< Window* >( glfwGetWindowUserPointer( window ) );
    app->setFramebufferResized( true );
}