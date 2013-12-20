include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
)

set(libhue_SRCS
    lights.cpp
)


### Library
add_library(libhue ${libhue_SRCS})
qt5_use_modules(libhue Gui)


### QML plugin
add_library(hueplugin MODULE ${libhue_SRCS} hueplugin.cpp)
qt5_use_modules(hueplugin Gui Qml Quick)

# Copy qmldir file to build dir for running from build dir
add_custom_target(hueplugin-qmldir ALL
    COMMAND cp ${CMAKE_CURRENT_SOURCE_DIR}/qmldir ${CMAKE_CURRENT_BINARY_DIR}
    DEPENDS ${QMLFILES}
)
