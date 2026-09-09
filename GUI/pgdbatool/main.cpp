#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

#if defined(Q_OS_WIN)
    // No Windows, força o estilo Fusion Claro para corrigir o bug do ecrã preto
    a.setStyle(QStyleFactory::create("Fusion"));

    // Repõe a palete de cores padrão clara do Qt
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

