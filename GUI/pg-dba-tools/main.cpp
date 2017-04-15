#include "mainwindow.h"
#include "dmoconnection.h"
#include <QApplication>
#include <QTranslator>
#include <QLocale>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    QString tfname;
    QTranslator trans;
    QString name;    
    QStringList params;
    DMOConnection db_conn;
    int ix = -1;

    name = QLocale::system().name();

    tfname = "pg-dba-tools" + QString::fromLatin1("_") + name + ".qm";
    if(trans.load(tfname))
        a.installTranslator(&trans);

    for(int i = 0; i < argc; i++)
        params << argv[i];

    if ((ix = params.indexOf("--host")) > 0 || (ix = params.indexOf("-h")) > 0)
        db_conn.setHost(params.at(ix+1));

    if ((ix = params.indexOf("--user")) > 0 || (ix = params.indexOf("-u")) > 0)
        db_conn.setUser(params.at(ix+1));

    if ((ix = params.indexOf("--password")) > 0 || (ix = params.indexOf("-p")) > 0)
        db_conn.setPassword(params.at(ix+1));

    if ((ix = params.indexOf("--port")) > 0 || (ix = params.indexOf("-P")) > 0)
        db_conn.setPort(params.at(ix+1).toInt());

    // Options like: --options "-c geqo=off"
    if ((ix = params.indexOf("--options")) > 0 || (ix = params.indexOf("-o")) > 0)
        db_conn.setOptions(params.at(ix+1));

    if (db_conn.connect()) {
        w.setDatabase(&db_conn);
        w.loadDatabases();
        w.show();
    }

    return a.exec();
}
