#-------------------------------------------------
#
# Project created by QtCreator 2020-09-08T21:09:04
#
#-------------------------------------------------

QT       += core gui xml

greaterThan(QT_MAJOR_VERSION, 5) {
    INCLUDEPATH += /home/marcotc/Qt/6.11.2/gcc_64/include
    DEPENDPATH  += /home/marcotc/Qt/6.11.2/gcc_64/include
}


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
    dlginsert.cpp \
    dlgupdate.cpp

HEADERS += \
        sqlgenerationplugin.h \
    dlginsert.h \
    dlgupdate.h
DISTFILES += pgdba_sqlgenplugin.json

INCLUDEPATH += ../pgdba_generators

# Include postgres lib
unix:INCLUDEPATH += $$system(pg_config --includedir)
unix:LIBS += -L$$system(pg_config --libdir)
unix:LIBS +=  -lpq

greaterThan(QT_MAJOR_VERSION, 5) {
    # Configuração para Qt 6
    INCLUDEPATH += /usr/include/qt6 \
                   /usr/include/x86_64-linux-gnu/qt6
    LIBS += -lqscintilla2_qt6
} else {
    # Configuração para Qt 5
    INCLUDEPATH += /usr/include/qt5 \
                   /usr/include/x86_64-linux-gnu/qt5
    LIBS += -lqscintilla2_qt5
}

INCLUDEPATH += ../pgdbatool

unix {
    target.path = /usr/lib
    INSTALLS += target
}

FORMS += \
    dlginsert.ui \
    dlgupdate.ui
