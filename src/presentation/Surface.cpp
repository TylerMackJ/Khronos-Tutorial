#include "Surface.hpp"

#include <stdexcept>

#include "HelloTriangleApplication.hpp"

using App = HelloTriangleApplication;

Surface::Surface()
{
	if( glfwCreateWindowSurface( App::get().getInstance().getInstanceRef(), App::get().getWindow().get(), nullptr, &surface) != VK_SUCCESS )
	{
		throw std::runtime_error( "failed to create window surface!" );
	}
}

Surface::~Surface()
{
	vkDestroySurfaceKHR( App::get().getInstance().getInstanceRef(), surface, nullptr);
}