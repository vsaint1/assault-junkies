cmake_minimum_required(VERSION 3.15)

project(assault_junkies)


add_library(${PROJECT_NAME} SHARED dllmain.cpp)

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_DIR}/external)
target_link_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/external/lib)

target_link_libraries(${PROJECT_NAME} PRIVATE OpenGL32)
