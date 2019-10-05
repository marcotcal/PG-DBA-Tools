#ifndef MODELSCANNER_H
#define MODELSCANNER_H

#include <QObject>
#include <QThread>

class ModelScanner : public QThread
{
    Q_OBJECT

public:
    ModelScanner(QString path, QObject *parent=nullptr);

protected:
    void run() override;

signals:
    void fileReaded(const QString &file_name);

private:
    QString path;
    bool isModel(const QString &file_name);

};

#endif // MODELSCANNER_H
