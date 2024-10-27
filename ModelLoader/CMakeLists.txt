cmake_minimum_required(VERSION 3.16)
project(ModelLoader CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS_DEBUG_INIT "-Wall -O0 -g")

find_package(Vulkan REQUIRED)

add_library(${PROJECT_NAME} STATIC
    src/Model.cpp
)

target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/include)

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/external)

target_link_libraries(${PROJECT_NAME} PRIVATE glfw)
target_link_libraries(${PROJECT_NAME} PRIVATE Vulkan::Vulkan)

install(TARGETS ${PROJECT_NAME} DESTINATION lib)