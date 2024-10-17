cmake_minimum_required(VERSION 3.16)
project(Kronos CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS_DEBUG_INIT "-Wall -O0 -g")

set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE INTERNAL "")

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

add_subdirectory(external/glfw-3.4)

find_package(Vulkan REQUIRED)

function(add_shaders TARGET_NAME)
    set(SHADER_SOURCE_FILES ${ARGN})

    list(LENGTH SHADER_SOURCE_FILES FILE_COUNT)
    if(FILE_COUNT EQUAL 0)
        message(FATAL_ERROR "No shader files specified")
    endif()

    foreach(SHADER_SOURCE IN LISTS SHADER_SOURCE_FILES)
        cmake_path(ABSOLUTE_PATH SHADER_SOURCE NORMALIZE)
        cmake_path(GET SHADER_SOURCE FILENAME SHADER_NAME)

        list(APPEND SHADER_COMMAND COMMAND)
        list(APPEND SHADER_COMMAND Vulkan::glslc)
        list(APPEND SHADER_COMMAND "${SHADER_SOURCE}")
        list(APPEND SHADER_COMMAND "-o")
        list(APPEND SHADER_COMMAND "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_BUILD_TYPE}/${SHADER_NAME}.spv")

        add_custom_target(${SHADER_NAME} ALL
            ${SHADER_COMMAND}
            COMMENT "Compiling Shaders [${SHADER_NAME}]"
            SOURCE ${SHADER_SOURCE}
            BYPRODUCTS "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_BUILD_TYPE}/${SHADER_NAME}.spv"
        )
    endforeach()

endfunction()

add_shaders(Kronos 
    shaders/shader.vert
    shaders/shader.frag
)

add_executable(Kronos 
    src/CommandBuffer.cpp
    src/CommandPool.cpp
    src/DebugMessenger.cpp
    src/Framebuffers.cpp
    src/GLFWInit.cpp
    src/GraphicsPipeline.cpp
    src/HelloTriangleApplication.cpp
    src/ImageViews.cpp
    src/Instance.cpp
    src/LogicalDevice.cpp
    src/PhysicalDevice.cpp
    src/RenderPass.cpp
    src/Surface.cpp
    src/SwapChain.cpp
    src/SyncObjects.cpp
    src/Window.cpp
    src/main.cpp
)
target_link_libraries(Kronos glfw)
target_link_libraries(Kronos Vulkan::Vulkan)