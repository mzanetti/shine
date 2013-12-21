file(GLOB_RECURSE QML_SRCS *.qml *.js)

set(shine_harmattan_SRCS
    main.cpp
    ${QML_SRCS}
)

add_executable(shine_harmattan ${shine_harmattan_SRCS})

target_link_libraries(shine_harmattan ${QT_QTCORE_LIBRARY} ${QT_QTDECLARATIVE_LIBRARY})

add_custom_target(shine_harmattan-qmlfiles ALL
    COMMAND cp -r ${CMAKE_CURRENT_SOURCE_DIR}/qml ${CMAKE_CURRENT_BINARY_DIR}
    DEPENDS ${QMLFILES}
)

install(TARGETS shine_harmattan RUNTIME DESTINATION opt/shine/bin)
install(FILES ${QML_SRCS} DESTINATION opt/shine/qml/)
install(FILES shine_harmattan.desktop DESTINATION usr/share/applications/)
