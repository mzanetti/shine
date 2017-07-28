TEMPLATE = lib
CONFIG += static

QT += network

TARGET = hue

HEADERS += action.h \
condition.h \
configuration.h \
discovery.h \
group.h \
groups.h \
huebridgeconnection.h \
huemodel.h \
hueobject.h \
light.h \
lightinterface.h \
lightsfiltermodel.h \
lights.h \
rule.h \
rulesfiltermodel.h \
rules.h \
scene.h \
scenesfiltermodel.h \
scenes.h \
schedule.h \
schedulesfiltermodel.h \
schedules.h \
sensor.h \
sensorsfiltermodel.h \
sensors.h \

SOURCES += action.cpp \
condition.cpp \
configuration.cpp \
discovery.cpp \
group.cpp \
groups.cpp \
huebridgeconnection.cpp \
huemodel.cpp \
hueobject.cpp \
light.cpp \
lights.cpp \
lightsfiltermodel.cpp \
rule.cpp \
rules.cpp \
rulesfiltermodel.cpp \
scene.cpp \
scenes.cpp \
scenesfiltermodel.cpp \
schedule.cpp \
schedules.cpp \
schedulesfiltermodel.cpp \
sensor.cpp \
sensors.cpp \
sensorsfiltermodel.cpp \
