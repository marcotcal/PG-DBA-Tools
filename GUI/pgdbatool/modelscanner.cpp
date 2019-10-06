#include "modelscanner.h"
#include <QDirIterator>

ModelScanner::ModelScanner(QString path, QObject *parent) :
    QThread(parent),
    path(path)
{

}

void ModelScanner::run()
{
    QString file_name;
    QDirIterator it(path, QDirIterator::Subdirectories);
    while (it.hasNext()) {
        file_name = it.next();
        if (checkFile(file_name))
            emit fileReaded(file_name);
    }
    saveModelList();
}

bool ModelScanner::checkFile(const QString &file_name)
{
    QFile file(file_name);
    QFileInfo file_info(file);
    QString model = "";
    QString code = "";
    QString description = "";
    QString model_path = "";

    if (file_info.suffix().toLower() != "xml")
        return false;

    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
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

                } else if (reader.name() == "model_path") {
                    model_path = reader.readElementText().trimmed();
                }
            }
        }
    }

    if (code == "")
        return false;

    models.append(new ModelListItem(file_name, code, description, model_path, this));

    reader.clear();
    file.close();
    return true;
}

void ModelScanner::saveModelList()
{
    QString DTD =
            "<!DOCTYPE models [\n"
            "  <!ELEMENT model (file_name, model_path)>\n"
            "  <!ATTLIST model code CDATA \"\">\n"
            "  <!ATTLIST model description CDATA \"\">\n"
            "  <!ELEMENT file_name (#PCDATA)>\n"
            "  <!ELEMENT model_path (#PCDATA)>\n"
            "]>\n";

    QFile file("models.xml");

    file.open(QIODevice::WriteOnly);
    QXmlStreamWriter xmlWriter(&file);
    xmlWriter.setAutoFormatting(true);
    xmlWriter.writeStartDocument();
    xmlWriter.writeDTD(DTD);

    xmlWriter.writeStartElement("models");

    foreach(ModelListItem *item, models) {
        xmlWriter.writeStartElement("model");
        xmlWriter.writeAttribute("","code", item->getCode());
        xmlWriter.writeAttribute("","description", item->getDescription());
        xmlWriter.writeStartElement("file_name");
        xmlWriter.writeCDATA(item->getFileName());
        xmlWriter.writeEndElement();
        xmlWriter.writeStartElement("model_path");
        xmlWriter.writeCDATA(item->getModelPath());
        xmlWriter.writeEndElement();
        xmlWriter.writeEndElement();
    }

    xmlWriter.writeEndElement();

    file.close();
}
