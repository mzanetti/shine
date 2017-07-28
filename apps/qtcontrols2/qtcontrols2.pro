TEMPLATE = app

QT += quick quickcontrols2 svg network

LIBS += -L ../../libhue -lhue

HEADERS += keystore.h \

SOURCES += keystore.cpp \
           main.cpp

RESOURCES += \
    resources.qrc


