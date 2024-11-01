#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <memory>

#include "DebugMessenger.hpp"
#include "Instance.hpp"

class Window
{
public:
    Window( int width, int height );
    ~Window();

    operator GLFWwindow*() { return window; }
    operator VkSurfaceKHR&() { return surface; }
    operator VkInstance&() { return *instance; }
    operator VkDebugUtilsMessengerEXT&() { return *debugMessenger; }

    const bool getFramebufferResized() const { return framebufferResized; }
    void setFramebufferResized( bool framebufferResized ) { this->framebufferResized = framebufferResized; }

    static void framebufferResizeCallback( GLFWwindow* window, int width, int height );

private:
    bool framebufferResized;

    GLFWwindow* window;
    VkSurfaceKHR surface;
    std::unique_ptr< Instance > instance;
    std::unique_ptr< DebugMessenger > debugMessenger;
};