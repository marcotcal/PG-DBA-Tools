#ifndef MODELSCANNER_H
#define MODELSCANNER_H

#include <QObject>
#include <QThread>
#include <QXmlStreamReader>

class ModelListItem : public QObject
{

    Q_OBJECT

public:

    ModelListItem(QString file_name, QString code, QString description, QString model_path, QObject *parent = nullptr) :
        QObject (parent),
        file_name(file_name),
        code(code),
        description(description),
        model_path(model_path)
    {

    }
    QString &getFileName() { return file_name; }
    QString &getCode() { return code; }
    QString &getDescription() { return description; }
    QString &getModelPath() { return model_path; }

private:

    QString file_name;
    QString code;
    QString description;
    QString model_path;

};

class ModelScanner : public QThread
{
    Q_OBJECT

public:
    ModelScanner(QString path, QObject *parent=nullptr);

protected:
    void run() override;

signals:
    void fileReaded(const QString &file_name);
    void scanCompleted();

private:
    QString path;
    QXmlStreamReader reader;
    QList<ModelListItem *> models;

    bool checkFile(const QString &file_name);
    void saveModelList();

};

#endif // MODELSCANNER_H
