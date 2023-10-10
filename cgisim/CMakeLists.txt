cmake_minimum_required(VERSION 3.27)

project(cgisim LANGUAGES CXX)


##C++17 due to usage of std::visit and std::variant
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)


###windows specific flags.
if(MSVC)
    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS TRUE)
    set(BUILD_SHARED_LIBS TRUE)
endif()

##import pybind11_add_module
add_subdirectory(pybind)

##we would use target_sources to make it clean
pybind11_add_module(cgisim SHARED "")


target_include_directories(cgisim PUBLIC ${PROJECT_SOURCE_DIR}/pybind/include)


## TODO: SHARED would


option(test "Set to compile test folder" OFF)

add_subdirectory(src)
target_include_directories(cgisim PUBLIC ${PROJECT_SOURCE_DIR}/include)

if (test)
    message("compiling test file")
    add_subdirectory(test)
endif()

