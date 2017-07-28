TEMPLATE = lib

QT += network
CONFIG += plugin c++11

TARGET = hueplugin

HEADERS += hueplugin.h \

SOURCES += hueplugin.cpp \

LIBS += -L../../libhue -lhue

OTHER_FILES = qmldir \
              ColorPicker.qml \
              ColorPickerCt.qml \

for(FILE, OTHER_FILES){
    QMAKE_POST_LINK += $$quote(cp $${PWD}/$${FILE} $${OUT_PWD}$$escape_expand(\\n\\t))
}
