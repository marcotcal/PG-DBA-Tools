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
        QXmlStreamReader reader(&file);

        while(!reader.atEnd() && !reader.hasError()) {

            QXmlStreamReader::TokenType token = reader.readNext();

            if(token == QXmlStreamReader::StartDocument) {
                continue;
            }
            if (reader.name() == "model") {
                QXmlStreamAttributes attributes = reader.attributes();
                if(attributes.hasAttribute("code"))
                    code = attributes.value("code").toString();
                if(attributes.hasAttribute("description"))
                    description = attributes.value("description").toString();
                while(!(reader.tokenType() == QXmlStreamReader::EndElement &&
                           reader.name().toString() == "model")) {
                    if(reader.tokenType() == QXmlStreamReader::StartElement) {
                        query_text = reader.text().toString().trimmed();
                    }
                }
            }
        }

        if(reader.hasError()) {
            error = true;
            error_message = reader.errorString();
            reader.clear();
            return false;
         }

         reader.clear();
         return true;

    } else {
        error = true;
        error_message = "no file name specified";
        return false;
    }
}

