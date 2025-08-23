cmake_minimum_required(VERSION 3.15)

project(Mortis LANGUAGES CXX)

include(cmake/config.cmake)
include(cmake/package.cmake)

add_subdirectory(core)
add_subdirectory(export) 
add_subdirectory(test) 