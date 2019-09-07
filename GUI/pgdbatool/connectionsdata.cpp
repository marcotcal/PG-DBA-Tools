#include "connectionsdata.h"
#include <QFile>
#include <QList>
#include <iostream>
#include <QXmlSimpleReader>

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

QMap<QString, QVariant> ConnectionElement::getParameters()
{
    return parameters;
}

QVariant ConnectionElement::getParameter(QString param)
{
    return parameters[param];
}

ConnectionsData::ConnectionsData()
{

}

ConnectionsData::~ConnectionsData()
{

}

bool ConnectionsData::readConnections()
{
    // Open a file for reading
    QFile file("connections.xml");
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        std::cout << "Failed to open the file for reading.";
        return false;
    }

    QXmlStreamReader reader(&file);

    if (reader.readNextStartElement()) {

        if (reader.name() == "connections") {

            while(reader.readNextStartElement()){
                if (reader.name().toString() == "connection") {
                    ConnectionElement *ce = new ConnectionElement();
                    QString name = reader.attributes().value("name").toString();
                    ce->setName(name);
                    while(reader.readNextStartElement()) {
                        QString param = reader.name().toString();
                        QString value = reader.readElementText().trimmed();
                        ce->addParameter(param, value);
                    }
                    connections.append(ce);
                }
            }
        }
    }

    return true;
}

void ConnectionsData::writeConnections()
{
    QFile file("connections.xml");
    QString DTD =
        "<!DOCTYPE connections [\n"
        "  <!ELEMENT connections (connection*)>\n"
        "  <!ELEMENT connection (host,dbname,port?,user?,password?)>\n"
        "  <!ATTLIST connection id CDATA \"\">\n"
        "  <!ATTLIST connection name CDATA \"\">\n"
        "  <!ELEMENT host (#PCDATA)>\n"
        "  <!ELEMENT dbname (#PCDATA)>\n"
        "  <!ELEMENT port (#PCDATA)>\n"
        "  <!ELEMENT user (#PCDATA)>\n"
        "  <!ELEMENT password (#PCDATA)>\n"
        "]>";

    file.open(QIODevice::WriteOnly);
    QXmlStreamWriter xmlWriter(&file);
    xmlWriter.setAutoFormatting(true);
    xmlWriter.writeStartDocument();
    xmlWriter.writeDTD(DTD);
    xmlWriter.writeStartElement("connections");

    Q_FOREACH(ConnectionElement *conn, connections) {
        xmlWriter.writeStartElement("connection");
        xmlWriter.writeAttribute("","name", conn->name());

        xmlWriter.writeStartElement("host");
        xmlWriter.writeCharacters(conn->getParameter("host").toString());
        xmlWriter.writeEndElement();

        xmlWriter.writeStartElement("dbname");
        xmlWriter.writeCharacters(conn->getParameter("dbname").toString());
        xmlWriter.writeEndElement();

        xmlWriter.writeStartElement("port");
        xmlWriter.writeCharacters(conn->getParameter("port").toString());
        xmlWriter.writeEndElement();

        xmlWriter.writeStartElement("user");
        xmlWriter.writeCharacters(conn->getParameter("user").toString());
        xmlWriter.writeEndElement();

        xmlWriter.writeStartElement("password");
        xmlWriter.writeCharacters(conn->getParameter("password").toString());
        xmlWriter.writeEndElement();

        xmlWriter.writeEndElement();
    }

    xmlWriter.writeEndElement();
    file.close();
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

