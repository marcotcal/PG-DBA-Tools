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

        QueryModelData *query_model = new QueryModelData();

        if (reader.readNextStartElement()) {

            if (reader.name() == "model") {

                while(reader.readNextStartElement()) {
                    if (reader.name().toString() == "code") {
                        code = reader.readElementText().trimmed();
                        query_model->setCode(code);
                    }
                    if (reader.name().toString() == "description") {
                        description = reader.readElementText().trimmed();
                        query_model->setDescription(description);
                    }
                    if (reader.name().toString() == "query_text") {
                        query_text = reader.readElementText().trimmed();
                        query_model->setDescription(query_text);
                    }
                    if (reader.name().toString() == "parameters") {
                        while(reader.readNextStartElement()) {
                            /*
                            if (reader.name().toString() == "parameter") {
                                QXmlStreamAttributes attr = reader.attributes();
                            }
                            */
                            a = reader.name().toString();
                        }
                    }
                    if (reader.name().toString() == "orders") {
                        while(reader.readNextStartElement()) {
                            QString param = reader.name().toString();
                            QString value = reader.readElementText().trimmed();
                        }
                    }


                }
            }
        }

        return true;

    } else {
        error = true;
        error_message = "no file name specified";
        return false;
    }
}

