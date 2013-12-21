if(QT4_BUILD)
    add_subdirectory(harmattan)
else()
    add_subdirectory(ubuntu)
    add_subdirectory(qtcontrols)
endif()

