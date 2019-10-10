#ifndef CONNECTIONSDATA_H
#define CONNECTIONSDATA_H

#include <QObject>
#include <QtXml>

class ConnectionElement : public QObject
{
public:
    ConnectionElement(QObject *parent = nullptr);
    ~ConnectionElement();
    QString name();
    void setName(QString name);
    void addParameter(QString param, QVariant value);
    QMap<QString, QVariant> getParameters();
    QVariant getParameter(QString param);
    QString connectStr();
    bool isInvalid() { return invalid; }
    void setInvalid(bool value) { invalid = value; }
private:    
    QString connection_name;
    bool invalid;
    QMap<QString, QVariant> parameters;
};

class ConnectionsData
{
public:
    ConnectionsData();
    ~ConnectionsData();
    bool readConnections();
    void writeConnections();
    QList<ConnectionElement *> &getConnections();
    ConnectionElement *newConnection();
    void sortByName();    
private:
    QList<ConnectionElement *> connections;
};

#endif // CONNECTIONSDATA_H
