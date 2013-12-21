include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/libhue
)

file(GLOB_RECURSE QML_SRCS *.qml *.js)

set(shine_SRCS
    main.cpp
    ${QML_SRCS}
)

add_executable(shine ${shine_SRCS})
qt5_use_modules(shine Gui Qml Quick)

target_link_libraries(shine hue)

add_custom_target(shine-qmlfiles ALL
    COMMAND cp -r ${CMAKE_CURRENT_SOURCE_DIR}/qml ${CMAKE_CURRENT_BINARY_DIR}
    DEPENDS ${QMLFILES}
)
