# CMakeLists.txt for stbi

cmake_minimum_required(VERSION 3.8)

project("stbi")

# Add the source files for the stbi library
add_library(stbi STATIC StbiWrite.cpp)

# Specify the include directories for the stbi library
target_include_directories(stbi PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
