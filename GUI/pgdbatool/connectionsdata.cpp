#include "connectionsdata.h"
#include <QFile>
#include <iostream>

ConnectionElement::ConnectionElement(int id, QString name)
{
    connection_id = id;
    connection_name = name;
}

ConnectionElement::~ConnectionElement()
{

}

int ConnectionElement::id()
{
    return connection_id;
}

QString ConnectionElement::name()
{
    return connection_name;
}


ConnectionsData::ConnectionsData()
{

}

void ConnectionsData::retrievElements(QDomElement root)
{
    QDomNodeList nodes = root.elementsByTagName("connection");

    std::cout << nodes.count();

    for(int i = 0; i < nodes.count(); i++)
    {
        QDomNode elm = nodes.at(i);
        if(elm.isElement())
        {
            QDomElement e = elm.toElement();
            qDebug() << e.attribute("id");
            QString name = e.attribute("name");
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

