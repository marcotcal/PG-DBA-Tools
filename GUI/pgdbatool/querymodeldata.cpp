#include "querymodeldata.h"
#include <QXmlStreamReader>
#include <QFileInfo>
#include <QMessageBox>

QueryModelData::QueryModelData(ConnectionsData &connections, ProjectData &project, QObject *parent) :
    QObject(parent),
    connections(connections),
    project(project)
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
    bool is_reading_parameter = false;
    QueryParameter *param = nullptr;

    while(!reader.atEnd()) {

        while(reader.readNext()) {

            if(reader.isStartDocument())
                continue;

            if (reader.isEndDocument())                
                break;

            if (reader.isEndElement()) {
                if (reader.name() == "parameter") {
                    is_reading_parameter = false;
                    param = nullptr;
                }
            }

            if (reader.isStartElement()) {

                if (is_reading_parameter) {
                    if (reader.name() == "expression") {
                         param->setExpression(reader.readElementText().trimmed());
                    }
                    if (reader.name() == "param_type") {
                         param->setType(reader.readElementText().trimmed());
                    }
                    if (reader.name() == "param_sub_type") {
                         param->setSubType(reader.readElementText().trimmed());
                    }
                }

                if (reader.name() == "model") {
                    QXmlStreamAttributes attributes = reader.attributes();
                    if(attributes.hasAttribute("code"))
                         code = attributes.value("code").toString();
                    if(attributes.hasAttribute("description"))
                         description = attributes.value("description").toString();
                } else if (reader.name() == "query_text") {
                    query_text = reader.readElementText().trimmed();
                } else if (reader.name() == "output_type") {
                    output_type = reader.readElementText().trimmed();
                } else if (reader.name() == "model_path") {
                    model_path = reader.readElementText().trimmed();
                } else if (reader.name() == "parameter") {
                    QXmlStreamAttributes attributes = reader.attributes();
                    QString code;
                    QString description;
                    QString mandatory;
                    if(attributes.hasAttribute("code"))
                         code = attributes.value("code").toString();
                    if(attributes.hasAttribute("description"))
                         description = attributes.value("description").toString();                    
                    if(attributes.hasAttribute("mandatory"))
                         mandatory = attributes.value("mandatory").toString();
                    param = new QueryParameter(code, description, (mandatory == "true"));
                    parameters.append(param);
                    is_reading_parameter = true;
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
            "  <!ELEMENT model (query_text, model_path, output_type, parameters?, orders?, columns?)>\n"
            "  <!ATTLIST model code CDATA \"\">\n"
            "  <!ATTLIST model description CDATA \"\">\n"
            "  <!ELEMENT query_text (#PCDATA)>\n"
            "  <!ELEMENT output_type (#PCDATA)>\n"
            "  <!ELEMENT model_path (#PCDATA)>\n"
            "  <!ELEMENT parameters (parameter*)>\n"
            "  <!ELEMENT parameter (expression, param_type, param_sub_type, widget?, str_options?, query_options?)>\n"
            "  <!ATTLIST parameter code CDATA \"\">\n"
            "  <!ATTLIST parameter description CDATA \"\">\n"            
            "  <!ATTLIST parameter mandatory (true|false) #REQUIRED>\n"
            "  <!ELEMENT widget (#PCDATA)>\n"
            "  <!ELEMENT str_options (#PCDATA)>\n"
            "  <!ELEMENT query_options (#PCDATA)>\n"
            "  <!ELEMENT param_sub_type (#PCDATA)>"
            "  <!ELEMENT param_type (#PCDATA)>"
            "  <!ELEMENT expression (#PCDATA)>"
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
        if (param->getMandatory())
            xmlWriter.writeAttribute("","mandatory", "true");
        else
            xmlWriter.writeAttribute("","mandatory", "false");

        xmlWriter.writeStartElement("expression");
        xmlWriter.writeCDATA(param->getExpression());
        xmlWriter.writeEndElement();

        xmlWriter.writeStartElement("param_type");
        xmlWriter.writeCDATA(param->getType());
        xmlWriter.writeEndElement();

        xmlWriter.writeStartElement("param_sub_type");
        xmlWriter.writeCDATA(param->getSubType());
        xmlWriter.writeEndElement();

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

    xmlWriter.writeStartElement("output_type");
    xmlWriter.writeCDATA(output_type);
    xmlWriter.writeEndElement();

    xmlWriter.writeStartElement("model_path");
    xmlWriter.writeCDATA(model_path);
    xmlWriter.writeEndElement();

    xmlWriter.writeEndElement();

    file.close();

    return true;
}

void QueryModelData::databaseConnect(int conn_number) {
    QMessageBox msg;
    PGresult *res;
    QString conn_str = connections.getConnections().at(conn_number)->connectStr();
    conn = PQconnectdb(conn_str.toStdString().c_str());

    if (PQstatus(conn) == CONNECTION_OK) {
        connected = true;
        res = PQexec(conn, "SET application_name=\"PGDBATool - Global Connection\"");
        connections.getConnections().at(conn_number)->setInvalid(false);
        return;
    }
    connections.getConnections().at(conn_number)->setInvalid(true);
    msg.setText(QString("Fail to Connect - %1").arg(PQerrorMessage(conn)));
    msg.exec();
    connected = false;
}

void QueryModelData::databaseDisconnect() {

    if (connected)
        PQfinish(conn);
    connected = false;
}

void QueryModelData::clear()
{
    while (!parameters.isEmpty())
        delete parameters.takeFirst();
    while (!orders.isEmpty())
        delete orders.takeFirst();
    while (!columns.isEmpty())
        delete columns.takeFirst();
    description = "";
    code = "";
    query_text = "";
}

QString QueryModelData::parseParameters(QString query)
{
    // remove the unused parameters
    QString ret = query;
    QString expr;
    QStringList items;

    foreach(QueryParameter *parameter, parameters) {

        if (parameter->getValue().isNull()) {

            ret.replace("[!" + parameter->getCode() + "!]", "");

        } else {

            expr = parameter->getExpression();
            if (parameter->getType() == "Text") {
                if (parameter->getSubType() == "Single") {
                    expr.replace("?", "'"+parameter->getValue().toString()+"'");
                } else if (parameter->getSubType() == "List") {
                    items = parameter->getValue().toString().split(";");
                    for(int i = 0; i < items.count(); i++)
                        items[i] = "'"+items[i]+"'";
                    expr.replace("?", "("+items.join(",")+")");
                } else if (parameter->getSubType() == "Range") {
                    // todo
                }
            }

            ret.replace("[!" + parameter->getCode() + "!]", expr);
        }

    }

    if (order_by != "")
        ret.replace("[!ORDER_BY!]", order_by);

    return ret;

}

void QueryModelData::execute(ResultOutput *output, bool show_query)
{
    QMessageBox msg;
    QString p_query = parseParameters(query_text);
    PGresult *res = PQexec(conn, p_query.toStdString().c_str());

    output->cleanMessage();

    switch(PQresultStatus(res)) {

    case PGRES_EMPTY_QUERY:
        output->clearOutput();
        output->generateStatusMessage(res);
        break;
    case PGRES_COMMAND_OK:
        output->clearOutput();
        if (show_query)
            output->generateStatusMessage(p_query+"\n");
        output->generateStatusMessage(res);
        break;
    case PGRES_TUPLES_OK:
        output->clearOutput();
        if (show_query)
            output->generateStatusMessage(p_query+"\n");
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
        if (show_query)
            output->generateStatusMessage(p_query+"\n");
        output->generateStatusMessage(res);
        break;
    case PGRES_FATAL_ERROR:
        output->clearOutput();
        output->generateError(conn);
        if (show_query)
            output->generateStatusMessage(p_query+"\n");
        output->generateStatusMessage(res);
        break;
    case PGRES_NONFATAL_ERROR:
        output->clearOutput();
        output->generateError(conn);
        if (show_query)
            output->generateStatusMessage(p_query+"\n");
        output->generateStatusMessage(res);
        break;
    }
    PQclear(res);
}

void QueryModelData::readModels()
{
    QString model_file_name;
    QFile file;
    QXmlStreamReader reader;
    QString code;
    QString description;
    QString file_name;
    QString model_path;

    if (project.getProjectName() == "")
        model_file_name = "models.xml";
    else
        model_file_name = QString("%1/config/models.xml").arg(project.getProjectPath());

    file.setFileName(model_file_name);

    while(!items.isEmpty())
        delete items.takeFirst();

    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        return;
    }

    reader.setDevice(&file);

    while(!reader.atEnd()) {

        while(reader.readNext()) {

            if(reader.isStartDocument())
                continue;

            if (reader.isEndDocument())
                break;

            if (reader.isEndElement()) {
                if (reader.name() == "model") {

                    items.append(new ModelItem(code, description, model_path, file_name, this));
                    code = "";
                    description = "";
                    file_name = "";
                    model_path = "";
                }
            }

            if (reader.isStartElement()) {

                if (reader.name() == "model") {
                    QXmlStreamAttributes attributes = reader.attributes();
                    if(attributes.hasAttribute("code"))
                         code = attributes.value("code").toString();
                    if(attributes.hasAttribute("description"))
                         description = attributes.value("description").toString();

                } else if (reader.name() == "model_path") {
                    model_path = reader.readElementText().trimmed();
                } else if (reader.name() == "file_name") {
                    file_name = reader.readElementText().trimmed();
                }

            }
        }
    }

   reader.clear();
   file.close();

}

