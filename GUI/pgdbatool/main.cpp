#include "mainwindow.h"
#include <QApplication>
#include <QStyleFactory>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

#if defined(Q_OS_WIN)
    // ignore for now dark style on windows
    a.setStyle(QStyleFactory::create("windowsvista"));
    QPalette lightPalette;
    a.setPalette(lightPalette);
#endif

    QCoreApplication::addLibraryPath(QCoreApplication::applicationDirPath() + "/plugins");

    QCoreApplication::setOrganizationName("Qvaliz");
    QCoreApplication::setApplicationName("PgDBATool");
    MainWindow w;
    w.show();

    return a.exec();
}

