#-------------------------------------------------
#
# Project created by QtCreator 2020-09-04T17:36:51
#
#-------------------------------------------------

QT       += widgets

TARGET = pgdba_generators
TEMPLATE = lib

DEFINES += PGDBA_GENERATORS_LIBRARY

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
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
    dlginsert.cpp

HEADERS += \
        pgdbagenerators.h \
        pgdba_generators_global.h \ 
    dlginsert.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

FORMS += \
    dlginsert.ui
