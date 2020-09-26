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
    PGresult *query_result;

    QStringList schemas(QString owner);
    QStringList users();
    bool executeSQL(const char *sql, const char *params[], int num_params);
    void clearResult();

};

#endif // PARAMETERBASE_H
