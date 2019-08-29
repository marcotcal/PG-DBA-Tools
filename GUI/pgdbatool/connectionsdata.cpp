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

void ConnectionsData::retrievElements(QDomElement root)
{
    QString name;

    QDomNodeList nodes = root.elementsByTagName("connection");

    for(int i = 0; i < nodes.count(); i++)
    {
        QDomNode ele = nodes.at(i);
        if (ele.isElement()) {
            QDomElement e = ele.toElement();
            if(e.tagName() == "connection") {
                name = e.attribute("name");
                ConnectionElement *ce = new ConnectionElement();                
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
        std::cout << "Failed to open the file for reading.";
        return false;
    }
    else
    {
        // loading
        if(!document.setContent(&file))
        {
            std::cout << "Failed to load the file for reading.";
            return false;
        }
        file.close();
    }

    // Getting root element
    QDomElement root = document.firstChildElement();

    retrievElements(root);
    return true;
}

QList<ConnectionElement *> ConnectionsData::getConnections()
{
    return connections;
}

ConnectionElement *ConnectionsData::newConnection()
{
    ConnectionElement *ele = new ConnectionElement();
    ele->setName("New Connection");
    ele->addParameter("host","localhost");
    ele->addParameter("port",5432);
    ele->addParameter("user","postgres");
    ele->addParameter("password","");
    ele->addParameter("dbname","postgres");

    connections.append(ele);
    return  ele;
}

void ConnectionsData::sortByName()
{
    std::sort(connections.begin(), connections.end(),
              [](ConnectionElement *a, ConnectionElement *b) -> bool { return a->name() < b->name(); });
}

