#include "Surface.hpp"

#include <stdexcept>

#include "HelloTriangleApplication.hpp"

Surface::Surface()
{
	if( glfwCreateWindowSurface( HelloTriangleApplication::getInstance().getInstanceRef(), HelloTriangleApplication::getWindow().get(), nullptr, &surface) != VK_SUCCESS )
	{
		throw std::runtime_error( "failed to create window surface!" );
	}
}

Surface::~Surface()
{
	vkDestroySurfaceKHR( HelloTriangleApplication::getInstance().getInstanceRef(), surface, nullptr);
}