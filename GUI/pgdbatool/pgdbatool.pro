#-------------------------------------------------
#
# Project created by QtCreator 2019-08-23T16:59:43
#
#-------------------------------------------------

QT       += core gui xml webenginewidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = pgdbatool

TEMPLATE = app

# This flag is necessary to mark the program as an executable.
# The behavior is occurring because newer ubuntu distros set GCC default link flag -pie, which marks
# e_type as ET_DYN on the binary file. Consequently, the Operating System recognizes as Shared Library.
QMAKE_LFLAGS += -no-pie

DEFINES += QT_DEPRECATED_WARNINGS

# disables all the APIs deprecated before Qt 6.0.0
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000

CONFIG += c++11

CONFIG(debug, debug|release) {
    DESTDIR = ../build/debug
}

CONFIG(release, debug|release) {
    DESTDIR = ../build/release
}

win32-* {
    TUNNELS=FALSE
} else {
    TUNNELS=TRUE
}

contains(TUNNELS, TRUE) {
    DEFINES += USE_SSH_TUNNELS
}

OBJECTS_DIR = $$DESTDIR/.obj
MOC_DIR = $$DESTDIR/.moc
RCC_DIR = $$DESTDIR/.qrc
UI_DIR = $$DESTDIR/.ui

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    sqltool.cpp \
    postgresqllexer.cpp \
    dlgconnections.cpp \
    connectionsdata.cpp \
    dlgmenunew.cpp \
    querymodel.cpp \
    dlgconfiguration.cpp \
    querymodeldata.cpp \
    resultoutput.cpp \
    plaintextoutput.cpp \
    xmltextoutput.cpp \
    jsonoutput.cpp \
    htmloutput.cpp \
    gridoutput.cpp \
    dlgtransaction.cpp \
    scitextoutput.cpp \
    dlgparameters.cpp \
    dlgexecutemodel.cpp \
    modelscanner.cpp \
    dlgexplain.cpp \
    outputset.cpp \
    queryexecutor.cpp \
    dlgplanmethods.cpp \
    dlggqoconfig.cpp \
    dlgparallelsettings.cpp \
    dlgworkers.cpp \
    dlgotherplanningsettings.cpp \
    connectionsettings.cpp \
    memoryeditor.cpp \
    dlgproject.cpp \
    projectdata.cpp \
    dlgcreatescript.cpp \
    frmfindtext.cpp \
    frmprojectscript.cpp \
    plugintabwidget.cpp \
    dlgplugins.cpp \
    pluginelement.cpp \
    plugintreewidget.cpp

HEADERS += \
    mainwindow.h \
    sqltool.h \
    postgresqllexer.h \
    dlgconnections.h \
    connectionsdata.h \
    dlgmenunew.h \
    querymodel.h \
    dlgconfiguration.h \
    querymodeldata.h \
    resultoutput.h \
    plaintextoutput.h \
    xmltextoutput.h \
    jsonoutput.h \
    htmloutput.h \
    gridoutput.h \
    dlgtransaction.h \
    scitextoutput.h \
    dlgparameters.h \
    dlgexecutemodel.h \
    modelscanner.h \
    dlgexplain.h \
    outputset.h \
    queryexecutor.h \
    dlgplanmethods.h \
    dlggqoconfig.h \
    dlgparallelsettings.h \
    dlgworkers.h \
    dlgotherplanningsettings.h \
    connectionsettings.h \
    memoryeditor.h \
    dlgproject.h \
    projectdata.h \
    dlgcreatescript.h \
    frmfindtext.h \
    frmprojectscript.h \
    plugintabwidget.h \
    pgdbaplugininterface.h \
    dlgplugins.h \
    pluginelement.h \
    plugintreewidget.h

FORMS += \
    mainwindow.ui \
    sqltool.ui \
    dlgconnections.ui \
    dlgmenunew.ui \
    querymodel.ui \
    dlgconfiguration.ui \
    dlgtransaction.ui \
    dlgparameters.ui \
    dlgexecutemodel.ui \
    dlgexplain.ui \
    outputset.ui \
    dlgplanmethods.ui \
    dlggqoconfig.ui \
    dlgparallelsettings.ui \
    dlgworkers.ui \
    dlgotherplanningsettings.ui \
    memoryeditor.ui \
    dlgproject.ui \
    dlgcreatescript.ui \
    frmfindtext.ui \
    frmprojectscript.ui \
    dlgplugins.ui

contains(TUNNELS, TRUE) {
    SOURCES += sshconnector.cpp
    HEADERS += sshconnector.h
}

INCLUDEPATH += ../pgdba_generators

LIBS += -lqscintilla2_qt5

# Include postgres lib
unix:INCLUDEPATH += $$system(pg_config --includedir)
unix:LIBS += -L$$system(pg_config --libdir)
unix:LIBS +=  -lpq

#include libssh2
contains(TUNNELS, TRUE) {
    unix:LIBS += -lssh2
}

RESOURCES += \
    pgdbatool.qrc

DISTFILES += \
    connections.xml \
    modelo.xml

contains(TUNNELS, TRUE) {
    message("compiled using ssh tunneling")
} else {
    message("compiled without using ssh tunneling")
}
