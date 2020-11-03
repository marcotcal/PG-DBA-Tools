#-------------------------------------------------
#
# Project created by QtCreator 2020-09-08T21:09:04
#
#-------------------------------------------------

QT       += core gui xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = pgdba_sqlgenplugin
TEMPLATE = lib
CONFIG += plugin

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
        sqlgenerationplugin.cpp \
    dlgparametersschema.cpp \
    dlgparameterobject.cpp

HEADERS += \
        sqlgenerationplugin.h \
    dlgparametersschema.h \
    dlgparameterobject.h
DISTFILES += pgdba_sqlgenplugin.json

# Include postgres lib
unix:INCLUDEPATH += $$system(pg_config --includedir)
unix:LIBS += -L$$system(pg_config --libdir)
unix:LIBS += $$system(pg_config --libs) -lpq

LIBS += -lqscintilla2_qt5

INCLUDEPATH += ../pgdbatool

unix {
    target.path = /usr/lib
    INSTALLS += target
}

FORMS += \
    dlgparametersschema.ui \
    dlgparameterobject.ui
