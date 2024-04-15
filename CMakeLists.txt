cmake_minimum_required(VERSION 3.15.0)
project(assaultjunkies)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)

set(CMAKE_CXX_STANDARD 17)

add_compile_options(/source-charset:utf-8 /execution-charset:utf-8)

file(GLOB_RECURSE MINHOOK ${CMAKE_CURRENT_SOURCE_DIR}/external/minhook/*.c)

file(GLOB_RECURSE SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)

add_library(${PROJECT_NAME} MODULE src/dllmain.cpp ${SOURCES} ${MINHOOK})

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/external)

target_link_libraries(${PROJECT_NAME} PRIVATE OpenGL32)
