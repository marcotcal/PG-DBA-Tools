#-------------------------------------------------
#
# Project created by QtCreator 2020-09-08T21:09:04
#
#-------------------------------------------------

QT       += core gui xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = pgdba_ddlgenplugin
TEMPLATE = lib
CONFIG += plugin
CONFIG += c++11

CONFIG(debug, debug|release) {
    DESTDIR = ../build/debug/plugins
}

CONFIG(release, debug|release) {
    DESTDIR = ../build/release/plugins
}

OBJECTS_DIR = $$DESTDIR/.obj
MOC_DIR = $$DESTDIR/.moc

RCC_DIR = $$DESTDIR/.qrc
UI_DIR = $$DESTDIR/.ui

DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += \
        ddlgenerationplugin.cpp

HEADERS += \
        ddlgenerationplugin.h
DISTFILES += pgdba_ddlgenplugin.json 

INCLUDEPATH += ../pgdba_generators

# Include postgres lib
unix:INCLUDEPATH += $$system(pg_config --includedir)
unix:LIBS += -L$$system(pg_config --libdir)
unix:LIBS +=  -lpq

unix:LIBS += -lqscintilla2_qt5

INCLUDEPATH += ../pgdbatool

unix {
    target.path = /usr/lib
    INSTALLS += target
}

FORMS += \
    dlgparametersschema.ui \
    dlgparametersequence.ui

# test windows compilation

# Include postgres lib
win32:INCLUDEPATH += $$system(pg_config --includedir)
win32:LIBS += -L$$system(pg_config --libdir)
win32:LIBS +=  -llibpq


win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../../Qt/6.1.2/msvc2019_64/lib/ -lqscintilla2_qt6
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../../Qt/6.1.2/msvc2019_64/lib/ -lqscintilla2_qt6d

INCLUDEPATH += $$PWD/../../../../Qt/6.1.2/msvc2019_64/include
DEPENDPATH += $$PWD/../../../../Qt/6.1.2/msvc2019_64/include
