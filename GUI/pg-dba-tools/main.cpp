#include "mainwindow.h"
#include <QApplication>
#include <QTranslator>
#include <QLocale>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    QString tfname;
    QString name;

    name = QLocale::system().name();

    QTranslator trans;

    tfname = "pg-dba-tools" + QString::fromLatin1("_") + name + ".qm";

    if(trans.load(tfname))
        a.installTranslator(&trans);

    w.show();

    return a.exec();
}
