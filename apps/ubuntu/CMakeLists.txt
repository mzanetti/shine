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
