cmake_minimum_required(VERSION 3.16)
project(Khronos_Binary CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS_DEBUG_INIT "-Wall -O0 -g")

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
            BYPRODUCTS "${CMAKE_CURRENT_BINARY_DIR}/${SHADER_NAME}.spv"
        )
    endforeach()

endfunction()

add_shaders(${PROJECT_NAME} 
    shaders/shader.vert
    shaders/shader.frag
)

configure_file(textures/texture.jpg "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_BUILD_TYPE}/textures/texture.jpg" COPYONLY)
configure_file(textures/viking_room.png "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_BUILD_TYPE}/textures/viking_room.png" COPYONLY)

configure_file(models/viking_room.obj "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_BUILD_TYPE}/models/viking_room.obj" COPYONLY)

add_executable(${PROJECT_NAME} 
    src/HelloTriangleApplication.cpp
    src/device/Device.cpp
    src/device/LogicalDevice.cpp
    src/device/PhysicalDevice.cpp
    src/drawing/CommandBuffer.cpp
    src/drawing/CommandPool.cpp
    src/drawing/Framebuffers.cpp
    src/drawing/SyncObjects.cpp
    src/graphicsPipeline/Buffer.cpp
    src/graphicsPipeline/BufferMemoryMap.cpp
    src/graphicsPipeline/DescriptorPool.cpp
    src/graphicsPipeline/DescriptorSetLayout.cpp
    src/graphicsPipeline/GraphicsPipeline.cpp
    src/graphicsPipeline/Image.cpp
    src/graphicsPipeline/RenderPass.cpp
    src/graphicsPipeline/TextureSampler.cpp
    src/main.cpp
    src/presentation/ImageView.cpp
    src/presentation/Surface.cpp
    src/presentation/SwapChain.cpp
    src/setup/DebugMessenger.cpp
    src/setup/GLFWInit.cpp
    src/setup/Instance.cpp
    src/setup/Window.cpp
)
target_precompile_headers(${PROJECT_NAME} PRIVATE
    src/HelloTriangleApplication.hpp
    src/device/Device.hpp
    src/device/LogicalDevice.hpp
    src/device/PhysicalDevice.hpp
    src/device/QueueFamilyIndices.hpp
    src/drawing/CommandBuffer.hpp
    src/drawing/CommandPool.hpp
    src/drawing/Framebuffers.hpp
    src/drawing/SyncObjects.hpp
    src/graphicsPipeline/Buffer.hpp
    src/graphicsPipeline/BufferMemoryMap.hpp
    src/graphicsPipeline/DescriptorPool.hpp
    src/graphicsPipeline/DescriptorSetLayout.hpp
    src/graphicsPipeline/GraphicsPipeline.hpp
    src/graphicsPipeline/Image.hpp
    src/graphicsPipeline/RenderPass.hpp
    src/graphicsPipeline/TextureSampler.hpp
    src/graphicsPipeline/UniformBufferObject.hpp
    src/presentation/ImageView.hpp
    src/presentation/Surface.hpp
    src/presentation/SwapChain.hpp
    src/setup/DebugMessenger.hpp
    src/setup/Instance.hpp
    src/setup/Window.hpp
)

target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/src)

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/external)

target_link_libraries(${PROJECT_NAME} glfw)
target_link_libraries(${PROJECT_NAME} Vulkan::Vulkan)
target_link_libraries(${PROJECT_NAME} ModelLoader)

install(TARGETS ${PROJECT_NAME} DESTINATION bin)
