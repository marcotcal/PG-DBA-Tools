#include "projectdata.h"
#include <QFile>
#include <QXmlStreamWriter>

ProjectData::ProjectData(QObject *parent) : QObject(parent)
{

}

QString ProjectData::getProjectPath() const
{
    return project_path;
}

void ProjectData::setProjectPath(const QString &value)
{
    project_path = value;
}

QString ProjectData::getProjectName() const
{
    return project_name;
}

void ProjectData::setProjectName(const QString &value)
{
    project_name = value;
}

QString ProjectData::getQueryPath() const
{
    return query_path;
}

void ProjectData::setQueryPath(const QString &value)
{
    query_path = value;
}

QString ProjectData::getModelPath() const
{
    return model_path;
}

void ProjectData::setModelPath(const QString &value)
{
    model_path = value;
}

QString ProjectData::getDevelopment() const
{
    return development;
}

void ProjectData::setDevelopment(const QString &value)
{
    development = value;
}

QString ProjectData::getStaging() const
{
    return staging;
}

void ProjectData::setStaging(const QString &value)
{
    staging = value;
}

QString ProjectData::getProduction() const
{
    return production;
}

void ProjectData::setProduction(const QString &value)
{
    production = value;
}

QString ProjectData::getDescription() const
{
    return description;
}

void ProjectData::setDescription(const QString &value)
{
    description = value;
}

void ProjectData::writeConfig()
{
    QFile file(project_path + "/config/config.xml");
    QString DTD =
        "<!DOCTYPE project [\n"
        "  <!ELEMENT config (project_path,project_name,development,staging,production,query_path,model_path,description)>\n"
        "  <!ELEMENT project_name (#PCDATA)>\n"
        "  <!ELEMENT project_path (#PCDATA)>\n"
        "  <!ELEMENT development (#PCDATA)>\n"
        "  <!ELEMENT staging (#PCDATA)>\n"
        "  <!ELEMENT production (#PCDATA)>\n"
        "  <!ELEMENT query_path (#PCDATA)>\n"
        "  <!ELEMENT model_path (#PCDATA)>\n"
        "  <!ELEMENT description (#PCDATA)>\n"
        "]>";

    file.open(QIODevice::WriteOnly);
    QXmlStreamWriter xmlWriter(&file);
    xmlWriter.setAutoFormatting(true);
    xmlWriter.writeStartDocument();
    xmlWriter.writeDTD(DTD);

    xmlWriter.writeStartElement("config");

    xmlWriter.writeStartElement("project_path");
    xmlWriter.writeCDATA(project_path);
    xmlWriter.writeEndElement();

    xmlWriter.writeStartElement("project_name");
    xmlWriter.writeCharacters(project_name);
    xmlWriter.writeEndElement();

    xmlWriter.writeStartElement("development");
    xmlWriter.writeCharacters(development);
    xmlWriter.writeEndElement();

    xmlWriter.writeStartElement("staging");
    xmlWriter.writeCharacters(staging);
    xmlWriter.writeEndElement();

    xmlWriter.writeStartElement("production");
    xmlWriter.writeCharacters(production);
    xmlWriter.writeEndElement();

    xmlWriter.writeStartElement("query_path");
    xmlWriter.writeCDATA(query_path);
    xmlWriter.writeEndElement();

    xmlWriter.writeStartElement("model_path");
    xmlWriter.writeCDATA(model_path);
    xmlWriter.writeEndElement();

    xmlWriter.writeStartElement("description");
    xmlWriter.writeCDATA(description);
    xmlWriter.writeEndElement();

    xmlWriter.writeEndElement();

    xmlWriter.writeEndElement();
    file.close();
}

bool ProjectData::readConfig()
{
    QString config_file;
    if (project_path == "")
        return false;

    config_file = project_path + "/config/config.xml";

    QFile file(config_file);
    QXmlStreamReader reader;

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

                if (reader.name() == "project_name") {
                    project_name = reader.readElementText().trimmed();
                } else if (reader.name() == "project_path") {
                    project_path = reader.readElementText().trimmed();
                } else if (reader.name() == "query_path") {
                    query_path = reader.readElementText().trimmed();
                }  else if (reader.name() == "model_path") {
                    model_path = reader.readElementText().trimmed();
                } else if (reader.name() == "description") {
                    description = reader.readElementText().trimmed();
                } else if (reader.name() == "development") {
                    development = reader.readElementText().trimmed();
                } else if (reader.name() == "staging") {
                    staging = reader.readElementText().trimmed();
                } else if (reader.name() == "production") {
                    production = reader.readElementText().trimmed();
                }
            }
        }
    }

    reader.clear();
    return true;

}
