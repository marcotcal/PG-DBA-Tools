#include "connectionsdata.h"
#include <QFile>
#include <QList>
#include <iostream>

ConnectionElement::ConnectionElement(QObject *parent) : QObject (parent)
{

}

ConnectionElement::~ConnectionElement()
{

}

int ConnectionElement::id()
{
    return connection_id;
}

void ConnectionElement::setId(int id)
{
    connection_id = id;
}

QString ConnectionElement::name()
{
    return connection_name;
}

void ConnectionElement::setName(QString name)
{
    connection_name = name;
}

void ConnectionElement::addParameter(QString param, QVariant value)
{
    parameters[param] = value;
}

QVariant ConnectionElement::parameter(QString param)
{
    return parameters[param];
}

ConnectionsData::ConnectionsData()
{

}

ConnectionsData::~ConnectionsData()
{

}

void ConnectionsData::retrieveElements(QDomElement root)
{
    int id;
    QString name;

    QDomNodeList nodes = root.elementsByTagName("connection");

    for(int i = 0; i < nodes.count(); i++)
    {
        QDomNode ele = nodes.at(i);
        if (ele.isElement()) {
            QDomElement e = ele.toElement();
            if(e.tagName() == "connection") {
                id = e.attribute("id").toInt();
                name = e.attribute("name");
                ConnectionElement *ce = new ConnectionElement();
                ce->setId(id);
                ce->setName(name);
                QDomNodeList cfgs = ele.childNodes();
                for (int j = 0; j < cfgs.count(); j++) {
                    QDomNode cfg = cfgs.at(j);
                    if (cfg.isElement()) {
                        QDomElement c = cfg.toElement();
                        ce->addParameter(c.tagName(), c.text().trimmed());
                    }
                }
                connections.append(ce);
            }
        }
    }
}

bool ConnectionsData::readConnections()
{
    QDomDocument document;

    // Open a file for reading
    QFile file("connections.xml");
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        return false;
    }
    else
    {
        // loading
        if(!document.setContent(&file))
        {
            return false;
        }
        file.close();
    }

    // Getting root element
    QDomElement root = document.firstChildElement();

    retrieveElements(root);
    return true;
}

QList<ConnectionElement *> ConnectionsData::getConnections()
{
    return connections;
}

