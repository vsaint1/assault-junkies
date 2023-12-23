cmake_minimum_required(VERSION 3.15)

project(assault_junkies)

set(CMAKE_GENERATOR_PLATFORM x86)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/build)


add_library(${PROJECT_NAME} MODULE dllmain.cpp)
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/external)
target_link_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/external/lib)

target_link_libraries(${PROJECT_NAME} PRIVATE OpenGL32 MinHook-x86)
