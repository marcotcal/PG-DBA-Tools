#include "projectdata.h"

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

}

void ProjectData::readConfig()
{

}
