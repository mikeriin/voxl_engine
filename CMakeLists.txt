cmake_minimum_required(VERSION 3.24)
project(voxl_superproject LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(voxl)
add_subdirectory(applications)