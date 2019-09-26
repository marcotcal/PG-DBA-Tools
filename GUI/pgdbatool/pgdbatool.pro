#-------------------------------------------------
#
# Project created by QtCreator 2019-08-23T16:59:43
#
#-------------------------------------------------

QT       += core gui xml webenginewidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
TARGET = pgdbatool
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

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
    scitextoutput.cpp

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
    scitextoutput.h

FORMS += \
    mainwindow.ui \
    sqltool.ui \
    dlgconnections.ui \
    dlgmenunew.ui \
    querymodel.ui \
    dlgconfiguration.ui \
    dlgtransaction.ui

LIBS += -lqscintilla2_qt5

unix:INCLUDEPATH += /usr/include/pgsql
unix:LIBS += -lpq -lpgcommon -lpgport -lssl -lcrypto -lgssapi_krb5 -lz -lrt -lcrypt -ldl -lm

RESOURCES += \
    pgdbatool.qrc

DISTFILES += \
    connections.xml \
    modelo.xml
