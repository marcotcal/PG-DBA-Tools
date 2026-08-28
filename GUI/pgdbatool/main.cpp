#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // Força o Qt 5 e Qt 6 a procurar os plugins na pasta ao lado do executável
    QCoreApplication::addLibraryPath(QCoreApplication::applicationDirPath() + "/plugins");

    QCoreApplication::setOrganizationName("Qvaliz");
    QCoreApplication::setApplicationName("PgDBATool");
    MainWindow w;
    w.show();

    return a.exec();
}

