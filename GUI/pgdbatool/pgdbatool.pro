#-------------------------------------------------
#
# Project created by QtCreator 2019-08-23T16:59:43
#
#-------------------------------------------------

QT       += core gui xml webenginewidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = pgdbatool

TEMPLATE = app

DEFINES += QT_DEPRECATED_WARNINGS

CONFIG += c++11

CONFIG(debug, debug|release) {
    DESTDIR = ../build/debug
}

CONFIG(release, debug|release) {
    DESTDIR = ../build/release
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

INCLUDEPATH += ../pgdba_generators

LIBS += -lqscintilla2_qt5

# Include postgres lib
unix:INCLUDEPATH += $$system(pg_config --includedir)
unix:LIBS += -L$$system(pg_config --libdir)
unix:LIBS +=  -lpq

# Include QSsh Libs

RESOURCES += \
    pgdbatool.qrc

DISTFILES += \
    connections.xml \
    modelo.xml
