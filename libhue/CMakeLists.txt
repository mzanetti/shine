include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
)

set(libhue_SRCS
    huebridgeconnection.cpp
    discovery.cpp
    configuration.cpp
    lights.cpp
    light.cpp
)

add_library(hue ${libhue_SRCS})

if(QT4_BUILD)
else()
    qt5_use_modules(hue Gui Network)
endif()

