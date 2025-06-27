cmake_minimum_required(VERSION 3.21)
set(CMAKE_CXX_STANDARD 20)
project(metal_cpp_try)


include(CMake/CMakeRC.cmake)

add_subdirectory(TinyObjLoader)
add_subdirectory(GLM)
add_subdirectory(metal-cmake)
add_subdirectory(Lib)
add_subdirectory(src)

