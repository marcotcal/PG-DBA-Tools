#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QCoreApplication::setOrganizationName("Qvaliz");
    QCoreApplication::setApplicationName("PgDBATool");
    MainWindow w;
    w.show();

    return a.exec();
}
