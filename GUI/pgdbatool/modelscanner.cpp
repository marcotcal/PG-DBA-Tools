#include "modelscanner.h"
#include <QDirIterator>

ModelScanner::ModelScanner(QString path, QObject *parent) :
    QThread(parent),
    path(path)
{

}

void ModelScanner::run()
{
    QString file_name;
    QDirIterator it(path, QDirIterator::Subdirectories);
    while (it.hasNext()) {
        file_name = it.next();
        if (data->isModel(file_name))
            emit fileReaded(file_name);
    }
}

bool ModelScanner::isModel(const QString &file_name)
{
    QFile file(file_name);
    QFileInfo file_info(file);

    if (file_info.suffix().toLower() != "xml")
        return false;

    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        return false;
    }

    file.close();
    return true;
}
