#include "mainwindow.h"
#include <QApplication>
#include <QStyleFactory>

int main(int argc, char *argv[])
{

#if defined(Q_OS_LINUX) || defined(Q_OS_UNIX)
        // dactivate hardware acelleration from Chromium to avoid problems with dma buffer
    // 1. Obriga o motor gráfico do Qt a renderizar por software pura (evita o dma_buf)
    qputenv("QT_QUICK_BACKEND", "software");

    // 2. Desativa o ecossistema de aceleração de hardware do Chromium por completo
    qputenv("QT_WEBENGINE_CHROMIUM_FLAGS", "--disable-gpu --disable-gpu-rasterization --disable-software-rasterizer --disable-gpu-compositing");
#endif

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

