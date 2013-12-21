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

# Copy all qml files and replace QtQuick 2.0 import with QtQuick 1.1
add_custom_target(CopyAndRename
    COMMAND ${CMAKE_COMMAND} -E copy ColorPicker.qml ColorPickerQtQuick1.qml && sed -i 's/import QtQuick 2.0/import QtQuick 1.1/' ColorPickerQtQuick1.qml
)


# Copy qmldir file to build dir for running from build dir
add_custom_target(hueplugin-qmlfiles ALL
    COMMAND cp ${QML_SRCS} ${CMAKE_CURRENT_BINARY_DIR}
    DEPENDS ${QMLFILES} CopyAndRename
)

if(HARMATTAN_BUILD)
install(TARGETS hueplugin LIBRARY DESTINATION opt/shine/plugins/Hue/)
install(FILES ${QML_SRCS} ${CMAKE_CURRENT_BINARY_DIR}/ColorPickerQtQuick1.qml DESTINATION opt/shine/plugins/Hue/)
endif()
