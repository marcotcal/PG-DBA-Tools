#ifndef PROJECTDATA_H
#define PROJECTDATA_H

#include <QObject>
#include "connectionsdata.h"

class ProjectData : public QObject
{
    Q_OBJECT
public:
    ProjectData(QObject *parent = nullptr);

    QString getProjectPath() const;
    void setProjectPath(const QString &value);

    QString getProjectName() const;
    void setProjectName(const QString &value);

    QString getQueryPath() const;
    void setQueryPath(const QString &value);

    QString getModelPath() const;
    void setModelPath(const QString &value);

    QString getDevelopment() const;
    void setDevelopment(const QString &value);

    QString getStaging() const;
    void setStaging(const QString &value);

    QString getProduction() const;
    void setProduction(const QString &value);

    QString getDescription() const;
    void setDescription(const QString &value);

    ConnectionsData &getConnnections() { return connections; }

    bool loadConnections();
    bool saveConnections();

    void clean();

    void writeConfig();
    bool readConfig();

private:

    ConnectionsData connections;
    QString project_path;
    QString project_name;
    QString query_path;
    QString model_path;
    QString development;
    QString staging;
    QString production;
    QString description;
};

#endif // PROJECTDATA_H
