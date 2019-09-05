#ifndef CONNECTIONSDATA_H
#define CONNECTIONSDATA_H

#include <QObject>
#include <QtXml>

class ConnectionElement : public QObject
{
public:
    ConnectionElement(QObject *parent = nullptr);
    ~ConnectionElement();
    int id();
    void setId(int id);
    QString name();
    void setName(QString name);
    void addParameter(QString param, QVariant value);
    QVariant parameter(QString param);
private:
    int connection_id;
    QString connection_name;
    QMap<QString, QVariant> parameters;
};

class ConnectionsData
{
public:
    ConnectionsData();
    ~ConnectionsData();
    bool readConnections();
    QList<ConnectionElement *> getConnections();
private:
    void retrieveElements(QDomElement root);
    QList<ConnectionElement *> connections;

};

#endif // CONNECTIONSDATA_H
