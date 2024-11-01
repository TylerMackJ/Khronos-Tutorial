#include "Device.hpp"

Device::Device(
    Window& window,
    std::vector< const char* > deviceExtensions,
    std::optional< const std::vector< const char* > > validationLayers
)
{
    physicalDevice = std::make_unique< PhysicalDevice >(
        window, std::set< std::string >( deviceExtensions.begin(), deviceExtensions.end() )
    );
    logicalDevice = std::make_unique< LogicalDevice >( *physicalDevice, deviceExtensions, validationLayers );
}