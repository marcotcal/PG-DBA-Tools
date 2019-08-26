#ifndef CONNECTIONSDATA_H
#define CONNECTIONSDATA_H

#include <QObject>
#include <QtXml>

class ConnectionElement
{
public:
    ConnectionElement(int id, QString name);
    ~ConnectionElement();
    int id();
    QString name();
private:
    int connection_id;
    QString connection_name;
};

class ConnectionsData
{
public:
    ConnectionsData();
    bool readConnections();
private:
    void retrievElements(QDomElement root);
};

#endif // CONNECTIONSDATA_H
