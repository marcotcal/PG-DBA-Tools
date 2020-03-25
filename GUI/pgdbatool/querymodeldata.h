#ifndef QUERYMODELDATA_H
#define QUERYMODELDATA_H
#include <QObject>
#include <QList>
#include <QXmlStreamReader>
#include <connectionsdata.h>
#include "resultoutput.h"
#include "projectdata.h"

class ModelItem : public QObject
{

    Q_OBJECT

public:
    ModelItem(QString code, QString description, QString menu_path,
              QString file_name, QObject *parent=nullptr) :
        QObject(parent),
        code(code),
        description(description),
        menu_path(menu_path),
        file_name(file_name)
    {

    }
    QString getCode() { return code;}
    QString getDescription() { return description; }
    QString getMenuPath() { return menu_path; }
    QString getFileName() { return file_name; }
    QStringList getMenuParts() { return menu_path.split("/", QString::SkipEmptyParts); }
private:
    QString code;
    QString description;
    QString menu_path;
    QString file_name;
};

class QueryParameter : public QObject
{

    Q_OBJECT

public:
    QueryParameter(QString code, QString description, bool mandatory, QObject *parent = nullptr) :
        QObject(parent), code(code), description(description), mandatory(mandatory)
    {

    }
    ~QueryParameter() { }
    QString &getCode() { return code; }
    QString &getDescription() { return description; }
    QString &getExpression() { return expression; }
    void setExpression(QString value) { expression = value; }
    QString &getType() { return param_type; }
    void setType(QString value) { param_type = value; }
    QString &getSubType() { return param_sub_type; }
    void setSubType(QString value) { param_sub_type = value; }
    bool &getMandatory() { return mandatory; }
    void setValue(QVariant value) { param_value = value; }
    QVariant &getValue() { return param_value; }

private:
    QString code;
    QString description;
    QString expression;
    QString param_type;
    QString param_sub_type;
    QVariant param_value;
    bool mandatory;
};

class QueryOrder : public QObject
{
    Q_OBJECT

public:
    QueryOrder(QString description, QString fields, QObject *parent = nullptr) :
        QObject(parent), description(description), fields(fields)
    {

    }
    ~QueryOrder() { }
    QString getDescription() { return description; }
    QString getFields() { return fields; }
private:
    QString description;
    QString fields;
};

class QueryColumn : public QObject
{

    Q_OBJECT

public:

    QueryColumn(QString title, int width, QObject *parent = nullptr) : QObject(parent),
        title(title), width(width)
    {

    }
    ~QueryColumn() { }
    QString getTitle() { return title; }
    int getWidth() { return width; }

private:
    QString title;
    int width;
};

class QueryModelData : public QObject
{

    Q_OBJECT

public:
    explicit QueryModelData(ConnectionsData &connections, ProjectData &project, QObject *parent = nullptr);
    ~QueryModelData();
    bool loadResource(QString resource);
    bool loadFromFile(QFile &file);
    QString getCode() { return code; }
    void setCode(QString value) { code = value; }
    QString getDescription() {return description; }
    void setDescription(QString value) { description = value; }
    QString getQueryText() { return query_text; }
    void setQueryText(QString value) { query_text = value; }
    QString getModelPath() { return model_path; }
    void setModelPath(QString value) { model_path = value; }
    QString getOutputType() { return output_type; }
    void setOutputType(QString value) { output_type = value; }
    bool getDatabaseRequest() { return database_request; }
    void setDatabaseRequest(bool value) { database_request = value; }
    QList <QueryParameter *> &getParameters() { return parameters; }
    QList <QueryOrder *> &getOrders() { return orders; }
    QList <QueryColumn *> &getColumns() { return columns; }
    void setOrderBy(QString value) { order_by = value; }
    bool getError() { return error; }
    QString getErrorMessage() { return error_message; }
    bool saveModel(QString file_name);
    void execute(ResultOutput* output, bool show_query = false);
    void databaseConnect(int conn_number, QString database_name);
    void databaseDisconnect();
    bool getConnected() { return connected; }
    void clear();
    void readModels();
    QList<ModelItem *> &getItems() { return items; }


private:
    QString code;
    QString description;
    QString query_text;
    QList<QueryParameter *> parameters;
    QList<QueryOrder *> orders;
    QList<QueryColumn *> columns;
    QList<ModelItem *> items;
    bool error;
    bool connected;
    QString order_by;
    QString error_message;
    QString model_path;
    QString output_type;
    bool database_request;
    QXmlStreamReader reader;
    ConnectionsData &connections;
    ProjectData &project;
    PGconn *conn;
    void readXML();
    QString parseParameters(QString query);   
};

#endif // QUERYMODELDATA_H
