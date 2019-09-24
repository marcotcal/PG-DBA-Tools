#ifndef QUERYMODELDATA_H
#define QUERYMODELDATA_H
#include <QObject>
#include <QList>
#include <QXmlStreamReader>
#include <connectionsdata.h>
#include "resultoutput.h"

class QueryParameter : public QObject
{

    Q_OBJECT

public:
    QueryParameter(QString code, QString description,
                   bool mandatory, QObject *parent = nullptr) :
        QObject(parent), code(code), description(description), mandatory(mandatory)
    {

    }
    ~QueryParameter() { }
    QString getCode() { return code; }
    QString getDescription() { return description; }
    bool getMandatory() { return mandatory; }

private:
    QString code;
    QString description;
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
    explicit QueryModelData(ConnectionsData &connections, QObject *parent = nullptr);
    ~QueryModelData();
    bool loadResource(QString resource);
    bool loadFromFile(QFile &file);
    QString getCode() { return code; }
    void setCode(QString value) { code = value; }
    QString getDescription() {return description; }
    void setDescription(QString value) { description = value; }
    QString getQueryText() { return query_text; }
    void setQueryText(QString value) { query_text = value; }
    QList <QueryParameter *> &getParameters() { return parameters; }
    QList <QueryOrder *> &getOrders() { return orders; }
    QList <QueryColumn *> &getColumns() { return columns; }
    bool getError() { return error; }
    QString getErrorMessage() { return error_message; }    
    bool saveModel(QString file_name);
    void execute(ResultOutput* output);
    void databaseConnect(int conn_number);
    void databaseDisconnect();

private:
    QString code;
    QString description;
    QString query_text;
    QList<QueryParameter *> parameters;
    QList<QueryOrder *> orders;
    QList<QueryColumn *> columns;
    bool error;
    bool connected;
    QString error_message;
    QXmlStreamReader reader;
    ConnectionsData &connections;
    PGconn *conn;
    void readXML();
};

#endif // QUERYMODELDATA_H
