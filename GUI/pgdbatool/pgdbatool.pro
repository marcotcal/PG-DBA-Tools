#-------------------------------------------------
#
# Project created by QtCreator 2019-08-23T16:59:43
#
#-------------------------------------------------

QT       += core gui xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

# Widget to be used as HML output ( if no option is selected QTexBrowser wil be used )
HTML = QWebView
#HTML = QWebEngineView


QT += webkit

equals(HTML, "QWebEngineView") {
    message("Use QWebEngineView widget on HML output")
    QT += webenginewidgets
    DEFINES += HML_USE_WEBENGINE
} else {
    equals(HTML, "QWebView") {
        message("Use QWebView widget on HML output")
        QT += webkitwidgets
        DEFINES += HML_USE_WEBKIT
    } else {
        message("Use QTextBrowser widget on HML output")
        DEFINES += HML_USE_TEXTBROWSER
    }
}

TARGET = pgdbatool

TEMPLATE = app

# This flag is necessary to mark the program as an executable.
# The behavior is occurring because newer ubuntu distros set GCC default link flag -pie, which marks
# e_type as ET_DYN on the binary file. Consequently, the Operating System recognizes as Shared Library.
QMAKE_LFLAGS += -no-pie

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
    dlgshowprocesses.cpp \
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
    dlgshowprocesses.h \
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
    dlgshowprocesses.ui \
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
#unix:LIBS += lQssh

RESOURCES += \
    pgdbatool.qrc

DISTFILES += \
    connections.xml \
    modelo.xml
