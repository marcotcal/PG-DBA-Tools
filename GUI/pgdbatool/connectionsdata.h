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
    QString connectStr(QString alternate_user="", QString alternate_password="", QString alternate_database="");
    bool isInvalid() { return invalid; }
    void setInvalid(bool value) { invalid = value; }
private:    
    QString connection_name;
    bool invalid;
    QMap<QString, QVariant> parameters;

    void openSSHTunnel();
};

class ConnectionsData
{
public:
    ConnectionsData();
    ~ConnectionsData();
    bool readConnections(QString path=".");
    void writeConnections(QString path=".");
    QList<ConnectionElement *> &getConnections();
    ConnectionElement *newConnection();
    void sortByName();    
    void clear();
private:
    QList<ConnectionElement *> connections;
};

#endif // CONNECTIONSDATA_H
