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

CONFIG(debug, debug|release) {
    DESTDIR = ../build/plugins
}

CONFIG(release, debug|release) {
    DESTDIR = ../build/plugins
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

# Inclui os subdiretórios locais do projeto
INCLUDEPATH += ../pgdba_generators \
               ../pgdbatool

# =================================================
# Configuração para Linux (Unix)
# =================================================
unix {
    INCLUDEPATH += $$system(pg_config --includedir)
    LIBS += -L$$system(pg_config --libdir) -lpq

    greaterThan(QT_MAJOR_VERSION, 5) {
        INCLUDEPATH += /usr/include/qt6 \
                       /usr/include/x86_64-linux-gnu/qt6
        LIBS += -lqscintilla2_qt6
    } else {
        INCLUDEPATH += /usr/include/qt5 \
                       /usr/include/x86_64-linux-gnu/qt5
        LIBS += -lqscintilla2_qt5
    }

    target.path = /usr/lib
    INSTALLS += target
}

# =================================================
# Configuração para Windows (MSYS2)
# =================================================
win32 {
    # Inclui rigorosamente todas as pastas de cabeçalhos do MSYS2
    INCLUDEPATH += "C:/dados/msys64/mingw64/include" \
                   "C:/dados/msys64/mingw64/include/qt6" \
                   "C:/dados/msys64/mingw64/include/qt6/Qsci"

    # Linka as bibliotecas do PostgreSQL e QScintilla do MSYS2
    LIBS += -L"C:/dados/msys64/mingw64/lib" -lqscintilla2_qt6 -lpq
}

FORMS += \
    dlgparametersschema.ui \
    dlgparametersequence.ui
