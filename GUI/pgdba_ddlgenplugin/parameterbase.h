#ifndef PARAMETERBASE_H
#define PARAMETERBASE_H

#include <QObject>
#include <libpq-fe.h>
#include <sqltool.h>

class ParameterBase
{
public:
    ParameterBase(PGconn *connection, EditorItem *editor);

protected:
    QString error;
    PGconn *connection;
    EditorItem *editor;

    int line;
    int index;
    int tuples;
    PGresult *resp;

    QStringList schemas(QString owner);
    QStringList users();
    void executeSQL(const char *sql, const char *params[], int num_params);
    virtual void readRows() = 0;

};

#endif // PARAMETERBASE_H
