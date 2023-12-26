cmake_minimum_required(VERSION 3.15)

project(assault_junkies)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_GENERATOR_PLATFORM x86)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/build)

file(GLOB_RECURSE MEM_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/mem/*.cpp)
file(GLOB_RECURSE CHEETOS_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/cheetos/*.cpp)
file(GLOB_RECURSE CHEETOS_HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/cheetos/*.hpp)

add_compile_options(/source-charset:utf-8 /execution-charset:utf-8)

add_library(${PROJECT_NAME} MODULE dllmain.cpp ${MEM_SOURCES} ${CHEETOS_SOURCES} ${CHEETOS_HEADERS})
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/external)
target_link_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/external/lib)

target_link_libraries(${PROJECT_NAME} PRIVATE OpenGL32 MinHook-x86)
