include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
)

set(libhue_SRCS
    huebridgeconnection.cpp
    discovery.cpp
    configuration.cpp
    lights.cpp
    light.cpp
    lightinterface.h
)

add_library(hue ${libhue_SRCS})

if(HARMATTAN_BUILD)
    target_link_libraries(hue qjson)
else()
    qt5_use_modules(hue Gui Network)
endif()

