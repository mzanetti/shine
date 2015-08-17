cmake_minimum_required(VERSION 2.8.9)
set(UBUNTU_MANIFEST_PATH "apps/ubuntu/manifest.json" CACHE INTERNAL "Tells QtCreator location and name of the manifest file")

project(shine C CXX)

set(CMAKE_AUTOMOC ON)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-permissive -pedantic -Wall -Wextra -fPIC")

include(FindPkgConfig)

if(HARMATTAN_BUILD)
    set(QT4_BUILD 1)
endif()

if(NOT QT4_BUILD)
    find_package(Qt5Core)
    find_package(Qt5Qml)
    find_package(Qt5Quick)
else()
    find_package(Qt4 REQUIRED)

    set(QT_USE_QTDECLARATIVE ON)
    set(QT_USE_QTNETWORK ON)

    include( ${QT_USE_FILE} )
endif()

add_subdirectory(libhue)
add_subdirectory(plugin)
add_subdirectory(apps)


