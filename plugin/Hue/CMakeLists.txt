include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/libhue
)
message(${CMAKE_SOURCE_DIR}/libhue)

file(GLOB_RECURSE QML_SRCS *.qml *.js qmldir)

add_library(hueplugin MODULE hueplugin.cpp ${QML_SRCS})

if(QT4_BUILD)
else()
qt5_use_modules(hueplugin Gui Qml Quick Network)
endif()

target_link_libraries(hueplugin hue)

# Copy qmldir file to build dir for running from build dir
add_custom_target(hueplugin-qmlfiles ALL
    COMMAND cp ${QML_SRCS} ${CMAKE_CURRENT_BINARY_DIR}
    DEPENDS ${QMLFILES}
)
