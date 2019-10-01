#include "querymodeldata.h"
#include <QXmlStreamReader>
#include <QFileInfo>
#include <QMessageBox>

QueryModelData::QueryModelData(ConnectionsData &connections, QObject *parent) :
    QObject(parent),
    connections(connections)
{
    error = false;
    connected = false;
    error_message = "";
}

QueryModelData::~QueryModelData()
{

}

void QueryModelData::readXML()
{
    while(!reader.atEnd()) {

        while(reader.readNext()) {

            if(reader.isStartDocument())
                continue;

            if (reader.isEndDocument())
                break;

            if (reader.isStartElement()) {

                if (reader.name() == "model") {
                    QXmlStreamAttributes attributes = reader.attributes();
                    if(attributes.hasAttribute("code"))
                         code = attributes.value("code").toString();
                    if(attributes.hasAttribute("description"))
                         description = attributes.value("description").toString();
                } else if (reader.name() == "query_text") {
                    query_text = reader.readElementText().trimmed();
                } else if (reader.name() == "parameter") {
                    QXmlStreamAttributes attributes = reader.attributes();
                    QString code;
                    QString description;
                    QString expression;
                    QString mandatory;
                    if(attributes.hasAttribute("code"))
                         code = attributes.value("code").toString();
                    if(attributes.hasAttribute("description"))
                         description = attributes.value("description").toString();
                    if(attributes.hasAttribute("expression"))
                         expression = attributes.value("expression").toString();
                    if(attributes.hasAttribute("mandatory"))
                         mandatory = attributes.value("mandatory").toString();
                    parameters.append(new QueryParameter(code, description, expression, (mandatory == "true")));
                } else if (reader.name() == "order") {
                    QXmlStreamAttributes attributes = reader.attributes();
                    QString fields;
                    QString description;

                    if(attributes.hasAttribute("description"))
                        description = attributes.value("description").toString();
                    if(attributes.hasAttribute("fields"))
                        fields = attributes.value("fields").toString();
                    orders.append(new QueryOrder(description, fields));
                }  else if (reader.name() == "column") {
                    QXmlStreamAttributes attributes = reader.attributes();
                    QString title;
                    int width = 100;

                    if(attributes.hasAttribute("title"))
                        title = attributes.value("title").toString();

                    if(attributes.hasAttribute("width"))
                        width = attributes.value("width").toInt();

                    columns.append(new QueryColumn(title, width));
                }
            }
        }
    }
}

bool QueryModelData::loadResource(QString resource)
{
    QFile file(resource);

    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        error = true;
        error_message = "could not open file";
        return false;
    }

    reader.setDevice(&file);
    readXML();
    reader.clear();
    return true;
}

bool QueryModelData::loadFromFile(QFile &file)
{  

    if(QFileInfo(file).suffix().toLower() == "xml") {

        if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            error = true;
            error_message = "could not open file";
            return false;
        }

        reader.setDevice(&file);
        readXML();
        reader.clear();
        return true;

    } else {
        error = true;
        error_message = "no file name specified";
        return false;
    }
}

bool QueryModelData::saveModel(QString file_name)
{
    QFile file(file_name);
    QString DTD =
            "<!DOCTYPE model [\n"
            "  <!ELEMENT model (query_text, parameters?, orders?, columns?)>\n"
            "  <!ATTLIST model code CDATA \"\">\n"
            "  <!ATTLIST model description CDATA \"\">\n"
            "  <!ELEMENT query_text (#PCDATA)>\n"
            "  <!ELEMENT parameters (parameter*)>\n"
            "  <!ELEMENT parameter (widget?, str_options?, query_options?)>\n"
            "  <!ATTLIST parameter code CDATA \"\">\n"
            "  <!ATTLIST parameter description CDATA \"\">\n"
            "  <!ATTLIST parameter expression CDATA \"\">"
            "  <!ATTLIST parameter mandatory (true|false) #REQUIRED>\n"
            "  <!ELEMENT widget (#PCDATA)>\n"
            "  <!ELEMENT str_options (#PCDATA)>\n"
            "  <!ELEMENT query_options (#PCDATA)>\n"
            "  <!ELEMENT orders (order*)>\n"
            "  <!ATTLIST order description CDATA \"\">\n"
            "  <!ATTLIST order fields CDATA \"\">\n"
            "  <!ELEMENT columns (column*)>\n"
            "  <!ATTLIST order title CDATA \"\">\n"
            "  <!ATTLIST order width CDATA \"\">\n"
            "]>\n";

    file.open(QIODevice::WriteOnly);
    QXmlStreamWriter xmlWriter(&file);
    xmlWriter.setAutoFormatting(true);
    xmlWriter.writeStartDocument();
    xmlWriter.writeDTD(DTD);

    xmlWriter.writeStartElement("model");
    xmlWriter.writeAttribute("","code", code);
    xmlWriter.writeAttribute("","description", description);

    xmlWriter.writeStartElement("parameters");
    Q_FOREACH(QueryParameter *param, parameters) {
        xmlWriter.writeStartElement("parameter");
        xmlWriter.writeAttribute("","code", param->getCode());
        xmlWriter.writeAttribute("","description", param->getDescription());
        xmlWriter.writeAttribute("","expression", param->getExpression());
        if (param->getMandatory())
            xmlWriter.writeAttribute("","mandatory", "true");
        else
            xmlWriter.writeAttribute("","mandatory", "false");
        xmlWriter.writeEndElement();
    }
    xmlWriter.writeEndElement();

    xmlWriter.writeStartElement("orders");
    Q_FOREACH(QueryOrder *order, orders) {
        xmlWriter.writeStartElement("order");
        xmlWriter.writeAttribute("","description", order->getDescription());
        xmlWriter.writeAttribute("","fields", order->getFields());
        xmlWriter.writeEndElement();
    }
    xmlWriter.writeEndElement();

    xmlWriter.writeStartElement("columns");
    Q_FOREACH(QueryColumn *column, columns) {
        xmlWriter.writeStartElement("column");
        xmlWriter.writeAttribute("","title", column->getTitle());
        xmlWriter.writeAttribute("","fields", QString("%1").arg(column->getWidth()));
        xmlWriter.writeEndElement();
    }
    xmlWriter.writeEndElement();

    xmlWriter.writeStartElement("query_text");
    xmlWriter.writeCDATA(query_text);
    xmlWriter.writeEndElement();

    xmlWriter.writeEndElement();

    file.close();

    return true;
}

void QueryModelData::databaseConnect(int conn_number) {
    QMessageBox msg;
    QString conn_str = connections.getConnections().at(conn_number)->connectStr();
    conn = PQconnectdb(conn_str.toStdString().c_str());

    if (PQstatus(conn) == CONNECTION_OK) {
        connected = true;
        return;
    }
    msg.setText(QString("Fail to Connect - %1").arg(PQerrorMessage(conn)));
    msg.exec();
    connected = false;
}

void QueryModelData::databaseDisconnect() {

    if (connected)
        PQfinish(conn);
    connected = false;
}

void QueryModelData::execute(ResultOutput *output)
{
    QMessageBox msg;

    PGresult *res = PQexec(conn, query_text.toStdString().c_str());

    output->cleanMessage();

    switch(PQresultStatus(res)) {

    case PGRES_EMPTY_QUERY:
        output->clearOutput();
        output->generateStatusMessage(res);
        break;
    case PGRES_COMMAND_OK:
        output->clearOutput();
        output->generateStatusMessage(res);
        break;
    case PGRES_TUPLES_OK:
        output->clearOutput();
        output->generateOutput(res);
        break;
    case PGRES_SINGLE_TUPLE:

        break;
    case PGRES_COPY_IN:

        break;
    case PGRES_COPY_OUT:

        break;
    case PGRES_COPY_BOTH:

        break;
    case PGRES_BAD_RESPONSE:
        output->clearOutput();
        output->generateError(conn);
        output->generateStatusMessage(res);
        break;
    case PGRES_FATAL_ERROR:
        output->clearOutput();
        output->generateError(conn);
        output->generateStatusMessage(res);
        break;
    case PGRES_NONFATAL_ERROR:
        output->clearOutput();
        output->generateError(conn);
        output->generateStatusMessage(res);
        break;
    }
    PQclear(res);
}

