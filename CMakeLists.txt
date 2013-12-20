cmake_minimum_required(VERSION 2.8.9)

project(shine C CXX)

set(CMAKE_AUTOMOC ON)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -fno-permissive -pedantic -Wall -Wextra")

include(FindPkgConfig)
find_package(Qt5Core)
find_package(Qt5Qml)
find_package(Qt5Quick)

add_subdirectory(libhue)
add_subdirectory(plugin)
add_subdirectory(apps)


