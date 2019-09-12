#include "querymodeldata.h"
#include <QXmlStreamReader>

QueryModelData::QueryModelData(QObject *parent) : QObject(parent)
{
    error = false;
    error_message = "";
}

QueryModelData::~QueryModelData()
{

}

bool QueryModelData::loadFromFile(QString file_name)
{  
    QString a;

    if(file_name != "") {

        QFile file(file_name);
        if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            error = true;
            error_message = "could not open file";
            return false;
        }
        reader.setDevice(&file);

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
                        QString mandatory;
                        if(attributes.hasAttribute("code"))
                             code = attributes.value("code").toString();
                        if(attributes.hasAttribute("description"))
                             description = attributes.value("description").toString();
                        if(attributes.hasAttribute("mandatory"))
                             mandatory = attributes.value("mandatory").toString();
                        parameters.append(new QueryParameter(code, description, (mandatory == "true")));
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

         reader.clear();
         return true;

    } else {
        error = true;
        error_message = "no file name specified";
        return false;
    }
}
