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
    src/HelloTriangleApplication.cpp
    src/HelloTriangleApplication.hpp
    src/device/LogicalDevice.cpp
    src/device/LogicalDevice.hpp
    src/device/PhysicalDevice.cpp
    src/device/PhysicalDevice.hpp
    src/drawing/CommandBuffer.cpp
    src/drawing/CommandBuffer.hpp
    src/drawing/CommandPool.cpp
    src/drawing/CommandPool.hpp
    src/drawing/Framebuffers.cpp
    src/drawing/Framebuffers.hpp
    src/drawing/SyncObjects.cpp
    src/drawing/SyncObjects.hpp
    src/graphicsPipeline/Buffer.cpp
    src/graphicsPipeline/Buffer.hpp
    src/graphicsPipeline/BufferMemoryMap.cpp
    src/graphicsPipeline/BufferMemoryMap.hpp
    src/graphicsPipeline/DescriptorPool.cpp
    src/graphicsPipeline/DescriptorPool.hpp
    src/graphicsPipeline/DescriptorSetLayout.cpp
    src/graphicsPipeline/DescriptorSetLayout.hpp
    src/graphicsPipeline/GraphicsPipeline.cpp
    src/graphicsPipeline/GraphicsPipeline.hpp
    src/graphicsPipeline/RenderPass.cpp
    src/graphicsPipeline/RenderPass.hpp
    src/graphicsPipeline/UniformBufferObject.hpp
    src/graphicsPipeline/Vertex.hpp
    src/main.cpp
    src/presentation/ImageViews.cpp
    src/presentation/ImageViews.hpp
    src/presentation/Surface.cpp
    src/presentation/Surface.hpp
    src/presentation/SwapChain.cpp
    src/presentation/SwapChain.hpp
    src/setup/DebugMessenger.cpp
    src/setup/DebugMessenger.hpp
    src/setup/GLFWInit.cpp
    src/setup/Instance.cpp
    src/setup/Instance.hpp
    src/setup/Window.cpp
    src/setup/Window.hpp
)
target_precompile_headers(Kronos
    PRIVATE
    src/HelloTriangleApplication.hpp
    src/device/LogicalDevice.hpp
    src/device/PhysicalDevice.hpp
    src/drawing/CommandBuffer.hpp
    src/drawing/CommandPool.hpp
    src/drawing/Framebuffers.hpp
    src/drawing/SyncObjects.hpp
    src/graphicsPipeline/Buffer.hpp
    src/graphicsPipeline/BufferMemoryMap.hpp
    src/graphicsPipeline/DescriptorPool.hpp
    src/graphicsPipeline/DescriptorSetLayout.hpp
    src/graphicsPipeline/GraphicsPipeline.hpp
    src/graphicsPipeline/RenderPass.hpp
    src/graphicsPipeline/UniformBufferObject.hpp
    src/graphicsPipeline/Vertex.hpp
    src/presentation/ImageViews.hpp
    src/presentation/Surface.hpp
    src/presentation/SwapChain.hpp
    src/setup/DebugMessenger.hpp
    src/setup/Instance.hpp
    src/setup/Window.hpp
)
target_include_directories(Kronos PRIVATE src)
target_include_directories(Kronos PUBLIC external/glm)
target_link_libraries(Kronos glfw)
target_link_libraries(Kronos Vulkan::Vulkan)