#-------------------------------------------------
#
# Project created by QtCreator 2020-09-08T21:09:04
#
#-------------------------------------------------

QT       += core gui

TARGET = pgdba_ddlgenplugin
TEMPLATE = lib
CONFIG += plugin

CONFIG(debug, debug|release) {
    DESTDIR = ../build/debug/plugins
}

CONFIG(release, debug|release) {
    DESTDIR = ../build/release/plugins
}

DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += \
        ddlgenerationplugin.cpp

HEADERS += \
        ddlgenerationplugin.h
DISTFILES += pgdba_ddlgenplugin.json 

# Include postgres lib
unix:INCLUDEPATH += $$system(pg_config --includedir)
unix:LIBS += -L$$system(pg_config --libdir)
unix:LIBS += $$system(pg_config --libs) -lpq

INCLUDEPATH += ../pgdbatool

unix {
    target.path = /usr/lib
    INSTALLS += target
}
