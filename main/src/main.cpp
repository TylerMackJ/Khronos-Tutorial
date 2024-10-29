#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include <vector>

#include "HelloTriangleApplication.hpp"

int main()
{
    try
    {
        HelloTriangleApplication::get().run();
    }
    catch( const std::exception& e )
    {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}