#-------------------------------------------------
#
# Project created by QtCreator 2017-04-08T08:37:19
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = pg-dba-tools
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    dlgconnections.cpp \
    dmoconnection.cpp

HEADERS  += mainwindow.h \
    dlgconnections.h \
    dmoconnection.h

FORMS    += mainwindow.ui \
    dlgconnections.ui

unix:INCLUDEPATH += /usr/include/pgsql
unix:LIBS += -lpq -lpgcommon -lpgport -lssl -lcrypto -lgssapi_krb5 -lz -lrt -lcrypt -ldl -lm
