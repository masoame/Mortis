cmake_minimum_required(VERSION 3.15)

project(Mortis LANGUAGES CXX)

include(CMake_cfg.cmake)
include(CMake_pkg.cmake)

add_subdirectory(core)
add_subdirectory(export) 
add_subdirectory(test) 


