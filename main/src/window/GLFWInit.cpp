#include "GLFWInit.hpp"

#include <iostream>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

GLFWInit::GLFWInit() { glfwInit(); }

GLFWInit::~GLFWInit() { glfwTerminate(); }