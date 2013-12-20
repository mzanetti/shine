include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/libhue
)

add_library(hueplugin MODULE hueplugin.cpp)
qt5_use_modules(hueplugin Gui Qml Quick Network)

target_link_libraries(hueplugin hue)

# Copy qmldir file to build dir for running from build dir
add_custom_target(hueplugin-qmldir ALL
    COMMAND cp ${CMAKE_CURRENT_SOURCE_DIR}/qmldir ${CMAKE_CURRENT_BINARY_DIR}
    DEPENDS ${QMLFILES}
)
