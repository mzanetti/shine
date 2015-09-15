include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
)

set(libhue_SRCS
    huebridgeconnection.cpp
    discovery.cpp
    configuration.cpp
    groups.cpp
    group.cpp
    lights.cpp
    lightsfiltermodel.cpp
    light.cpp
    lightinterface.h
    scenes.cpp
    scene.cpp
    schedules.cpp
    schedule.cpp
    scenesfiltermodel.cpp
    schedulesfiltermodel.cpp
)

add_library(hue ${libhue_SRCS})

if(HARMATTAN_BUILD)
    target_link_libraries(hue qjson)
else()
    qt5_use_modules(hue Gui Network)
endif()

