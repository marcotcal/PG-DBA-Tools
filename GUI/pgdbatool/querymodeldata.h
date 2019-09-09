#ifndef QUERYMODELDATA_H
#define QUERYMODELDATA_H
#include <QObject>
#include <QList>
#include <QDomElement>
\
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

class QueryModelData : public QObject
{

    Q_OBJECT

public:
    explicit QueryModelData(QObject *parent = nullptr);
    ~QueryModelData();
    bool loadResource(QString resource);
    bool loadFromFile(QString file_name);
    QString getCode() { return code; }
    void setCode(QString value) { code = value; }
    QString getDescription() {return description; }
    void setDescription(QString value) { description = value; }
    QString getQueryText() { return query_text; }
    void setQueryText(QString value) { query_text = value; }
    QList <QueryParameter *> getParameters() { return parameters; }
    bool getError() { return error; }
    QString getErrorMessage() { return error_message; }
private:
    QString code;
    QString description;
    QString query_text;
    QList<QueryParameter *> parameters;
    void retrieveElements(QDomElement root);
    bool error;
    QString error_message;
};

#endif // QUERYMODELDATA_H
