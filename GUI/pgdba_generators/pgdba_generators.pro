#-------------------------------------------------
#
# Project created by QtCreator 2020-09-04T17:36:51
#
#-------------------------------------------------

QT       += widgets gui xml

TARGET = pgdba_generators
TEMPLATE = lib
CONFIG += staticlib

DEFINES += PGDBA_GENERATORS_LIBRARY

DEFINES += QT_DEPRECATED_WARNINGS

#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG(debug, debug|release) {
    DESTDIR = ../build/debug
}

CONFIG(release, debug|release) {
    DESTDIR = ../build/release
}

# Include postgres lib
unix:INCLUDEPATH += $$system(pg_config --includedir)
unix:LIBS += -L$$system(pg_config --libdir)
unix:LIBS += $$system(pg_config --libs) -lpq

OBJECTS_DIR = $$DESTDIR/.obj
MOC_DIR = $$DESTDIR/.moc
RCC_DIR = $$DESTDIR/.qrc
UI_DIR = $$DESTDIR/.ui

SOURCES += \
    pgdbagenerators.cpp \
    parameterbase.cpp

HEADERS += \
    pgdbagenerators.h \
    pgdba_generators_global.h \
    parameterbase.h

LIBS += -lqscintilla2_qt5

INCLUDEPATH += ../pgdbatool

unix {
    target.path = /usr/lib
    INSTALLS += target
}

FORMS +=
