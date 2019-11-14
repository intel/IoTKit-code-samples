cmake_minimum_required (VERSION 3.0)
project (ANALOG-IN)

# OS checks
if(WIN32)
    message(FATAL_ERROR, "This sample cannot be compiled natively on Windows OS.")
endif()

# Dependency checks
find_package(PkgConfig)
pkg_search_module (MRAA REQUIRED mraa)

add_subdirectory(cpp)
