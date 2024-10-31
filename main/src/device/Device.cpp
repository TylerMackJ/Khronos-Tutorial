#include "Device.hpp"

Device::Device()
{
    physicalDevice = std::make_unique< PhysicalDevice >();
    logicalDevice = std::make_unique< LogicalDevice >(
        *physicalDevice,
        std::vector< const char* >( { VK_KHR_SWAPCHAIN_EXTENSION_NAME } )
#ifndef NDEBUG
            ,
        std::optional< const std::vector< const char* > >( { "VK_LAYER_KHRONOS_validation" } )
#endif
    );
}