#include "querymodeldata.h"

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
    QDomDocument document;

    if(file_name != "") {

        QFile file(file_name);
        if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            error = true;
            error_message = "could not open file";
            return false;
        }
        else
        {
            if(!document.setContent(&file))
            {
                error = true;
                error_message = "invalid xml file";
                return false;
            }
            file.close();
        }

        QDomElement root = document.firstChildElement();
        retrieveElements(root);

        error = false;
        return true;

    }

    error = true;
    error_message = "no file name specified";
    return false;
}

bool QueryModelData::loadResource(QString resource)
{
    // example of resource ":/query_defs/query_models/modelo.xml"
    QDomDocument document;

    QString file_name = QString(resource);
    QFile file(file_name);
    if(!file.open(QIODevice::ReadOnly)) {
        return false;
    }
    document.setContent(&file);

    QDomElement root = document.firstChildElement();
    retrieveElements(root);
}

void QueryModelData::retrieveElements(QDomElement root)
{
    QDomElement tag_parameters = root.elementsByTagName("parameters").at(0).toElement();
    QDomNodeList nodes = tag_parameters.elementsByTagName("parameter");
    QueryParameter *item;
    QString param_code; 
    QString param_description;
    bool param_mandatory;

    for(int i = 0; i < nodes.count(); i++) {
        QDomNode ele = nodes.at(i);
        if (ele.isElement()) {
            QDomElement e = ele.toElement();
            if(e.tagName() == "parameter") {
                param_code = e.attribute("code");                
                param_description = e.attribute("description");                
                if(e.attribute("mandatory") == "true")
                    param_mandatory = true;
                else
                    param_mandatory = false;
                item = new  QueryParameter(param_code, param_description, param_mandatory);
                parameters.append(item);
            }
        }
    }
    QDomElement ele;
    ele = root.elementsByTagName("code").at(0).toElement();
    code = ele.text();
    ele = root.elementsByTagName("description").at(0).toElement();
    description = ele.text();
    ele = root.elementsByTagName("query_text").at(0).toElement();
    query_text = ele.text();
}
