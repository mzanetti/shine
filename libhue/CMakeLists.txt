include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
)

set(libhue_SRCS
    huebridgeconnection.cpp
    hueobject.cpp
    huemodel.cpp
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
    sensor.cpp
    sensors.cpp
    sensorsfiltermodel.cpp
    rule.cpp
    rules.cpp
    rulesfiltermodel.cpp
    action.cpp
    condition.cpp
)

add_library(hue ${libhue_SRCS})

if(HARMATTAN_BUILD)
    target_link_libraries(hue qjson)
else()
    qt5_use_modules(hue Gui Network)
endif()

