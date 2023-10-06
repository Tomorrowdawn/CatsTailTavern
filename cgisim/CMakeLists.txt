cmake_minimum_required(VERSION 3.27)

project(cgisim LANGUAGES CXX)


##C++17 due to usage of std::visit and std::variant
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)


##import pybind11_add_module
add_subdirectory(pybind)

##we would use target_sources to make it clean
pybind11_add_module(cgisim "")

option(test "Set to compile unitest folder" OFF)

add_subdirectory(src)

if (test)
    message("compiling test file")
    add_subdirectory(unitest)
