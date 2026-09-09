#-------------------------------------------------
#
# Project created by QtCreator 2019-08-23T16:59:43
#
#-------------------------------------------------

QT += core gui xml

win32 {
    # Flag para resolver o conflito de runtime do GCC/MinGW
    QMAKE_LFLAGS += -Wl,--allow-multiple-definition
}

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

# =================================================
# HTML output widget
#
# QWebEngineView  -> Qt WebEngine
# QWebView        -> Qt WebKit (Qt 5 only)
# Anything else   -> QTextBrowser
# =================================================

# Default HTML widget
HTML = QWebEngineView

win32 {
    HTML = QTextBrowser
}

equals(HTML, "QWebEngineView") {
    message("Use QWebEngineView widget on HTML output")
    QT += webenginewidgets
    DEFINES += HML_USE_WEBENGINE
} else:equals(HTML, "QWebView") {
    greaterThan(QT_MAJOR_VERSION, 5) {
        # QWebView/QWebKit does not exist in Qt 6.
        # Fall back to QTextBrowser.
        message("QWebView is not available with Qt 6 - using QTextBrowser")
        DEFINES += HML_USE_TEXTBROWSER
    } else {
        # Qt 5
        message("Use QWebView widget on HTML output")
        QT += webkitwidgets
        DEFINES += HML_USE_WEBKIT
    }
} else {
    message("Use QTextBrowser widget on HTML output")
    DEFINES += HML_USE_TEXTBROWSER
}

# =================================================
# QScintilla & PostgreSQL (Dependências Externas)
# =================================================

unix {

    INCLUDEPATH += $$system(pg_config --includedir)
    LIBS += -L$$system(pg_config --libdir) -lpq

    greaterThan(QT_MAJOR_VERSION, 5) {
        message("Using QScintilla and includes strictly from Qt 6.11.2 Local Path")

        # OBRIGA o compilador a ler APENAS os cabeçalhos da sua pasta 6.11.2
        INCLUDEPATH += /home/marcotc/Qt/6.11.2/gcc_64/include \
                      /home/marcotc/Qt/6.11.2/gcc_64/include/QtCore \
                      /home/marcotc/Qt/6.11.2/gcc_64/include/QtWidgets \
                      /usr/include/qt6 \
                      /usr/include/x86_64-linux-gnu/qt6 \
                      .

        DEPENDPATH  = /home/marcotc/Qt/6.11.2/gcc_64/include

        LIBS += -L/home/marcotc/Qt/6.11.2/gcc_64/lib -lqscintilla2_qt6
    } else {
        # Qt 5 Linux
        message("Using QScintilla for Qt 5")
        INCLUDEPATH += /usr/include/qt5 \
                       /usr/include/x86_64-linux-gnu/qt5 \
                       .
        LIBS += -lqscintilla2_qt5
    }
}

win32 {
    message("Using QScintilla and PostgreSQL from MSYS2 Paths")

    # Inclui rigorosamente todas as pastas de cabeçalhos necessárias do MSYS2
    INCLUDEPATH += "C:/dados/msys64/mingw64/include" \
                   "C:/dados/msys64/mingw64/include/qt6" \
                   "C:/dados/msys64/mingw64/include/qt6/Qsci"

    # Junta o caminho de linkagem da biblioteca do QScintilla e do PostgreSQL num único comando limpo
    LIBS += -L"C:/dados/msys64/mingw64/lib" -lqscintilla2_qt6 -lpq

    # Força a inclusão estática do runtime para evitar o conflito do crt2.o
    QMAKE_LFLAGS += -static-libgcc -static-libstdc++
    LIBS += -lmingw32 -lgcc -lmoldname -lmingwex -lmsvcrt
}
unix {
    # This flag is necessary to mark the program as an executable.
    QMAKE_LFLAGS += -no-pie
}

# =================================================
# Application
# =================================================

TARGET = pgdbatool
TEMPLATE = app

# This flag is necessary to mark the program as an executable.
QMAKE_LFLAGS += -no-pie

DEFINES += QT_DEPRECATED_WARNINGS

# =================================================
# C++ standard
# =================================================

greaterThan(QT_MAJOR_VERSION, 5) {
    CONFIG += c++20
    CONFIG -= c++17
    CONFIG -= c++11

    QMAKE_CXXFLAGS += -std=c++20 -fpermissive
    QMAKE_CFLAGS   += -std=c17
} else {
    CONFIG += c++11
}

# =================================================
# Build directories
# =================================================

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

# =================================================
# Sources
# =================================================

SOURCES += \
    dlgpassword.cpp \
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

# =================================================
# Headers
# =================================================

HEADERS += \
    dlgpassword.h \
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

# =================================================
# Forms
# =================================================

FORMS += \
    dlgpassword.ui \
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

# =================================================
# Include PGDBA generators
# =================================================

INCLUDEPATH += ../pgdba_generators \

# =================================================
# Resources
# =================================================

RESOURCES += \
    pgdbatool.qrc

# =================================================
# Distribution files
# =================================================

DISTFILES += \
    connections.xml \
    modelo.xml

