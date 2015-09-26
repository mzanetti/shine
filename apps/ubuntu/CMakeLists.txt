file(GLOB_RECURSE QML_SRCS *.qml *.js)

set(shine_ubuntu_SRCS
    main.cpp
    keystore.cpp
    ${QML_SRCS}
)

add_executable(shine_ubuntu ${shine_ubuntu_SRCS})

qt5_use_modules(shine_ubuntu Gui Qml Quick)

add_custom_target(shine_ubuntu-qmlfiles ALL
    COMMAND cp -r ${CMAKE_CURRENT_SOURCE_DIR}/qml ${CMAKE_CURRENT_BINARY_DIR}
    DEPENDS ${QMLFILES}
)

file(GLOB QML_FILES qml/*.qml)
file(GLOB IMAGE_FILES qml/images/*)

set(PLUGIN_FILES
    ${CMAKE_BINARY_DIR}/plugin/Hue/libhueplugin.so
    ${CMAKE_BINARY_DIR}/plugin/Hue/ColorPicker.qml
    ${CMAKE_BINARY_DIR}/plugin/Hue/ColorPickerCt.qml
    ${CMAKE_BINARY_DIR}/plugin/Hue/LightRecipeModel.qml
    ${CMAKE_BINARY_DIR}/plugin/Hue/qmldir
)

install(FILES ${PLUGIN_FILES} DESTINATION /Hue/)
install(FILES "shine.json" DESTINATION /)
install(FILES "manifest.json" DESTINATION /)
install(FILES "shine.desktop" DESTINATION /)
install(FILES "shine.svg" DESTINATION /)
install(FILES ${QML_FILES} DESTINATION /qml/)
install(FILES ${IMAGE_FILES} DESTINATION /qml/images)
install(TARGETS shine_ubuntu DESTINATION /)
