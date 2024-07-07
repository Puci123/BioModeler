# CMakeLists.txt for stbi

cmake_minimum_required(VERSION 3.8)

project("stbi")

# Create an interface library for the header-only library
add_library(stbi INTERFACE)

# Specify the include directories for the stbi library
target_include_directories(stbi INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
