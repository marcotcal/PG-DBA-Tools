#include "parameterbase.h"
#include <QMessageBox>

ParameterBase::ParameterBase(PGconn *connection, EditorItem *editor) :
    connection(connection),
    editor(editor)
{
    error = "";
    if (editor)
        editor->getCursorPosition(&line, &index);
    else
        line = index = 0;
}

QStringList ParameterBase::schemas(QString owner)
{
    QString sql =
        "SELECT schema_name "
        "FROM information_schema.schemata "
        "WHERE schema_name NOT IN ('information_schema') AND schema_name !~ '^pg_' ";


    if(!owner.isEmpty()) {
        sql += QString("AND schema_owner = '%1' ").arg(owner);
    }

    QStringList list;

    list << "";

    int tuples;

    if (PQstatus(connection) == CONNECTION_OK) {
        PGresult *res = PQexec(connection, sql.toStdString().c_str());

        if (PQresultStatus(res) != PGRES_TUPLES_OK)
        {
            PQclear(res);
        } else {

            tuples = PQntuples(res);

            for (int i = 0; i < tuples; i++)
                list << QString::fromStdString(PQgetvalue(res, i, 0));

            PQclear(res);

            return list;

        }
    }

    return QStringList();
}

QStringList ParameterBase::users()
{
    const char *sql =
            "SELECT usename AS role_name, "
            "  CASE "
            "     WHEN usesuper AND usecreatedb THEN "
            "       CAST('superuser, create database' AS pg_catalog.text) "
            "     WHEN usesuper THEN "
            "        CAST('superuser' AS pg_catalog.text) "
            "     WHEN usecreatedb THEN "
            "        CAST('create database' AS pg_catalog.text) "
            "     ELSE "
            "        CAST('' AS pg_catalog.text) "
            "  END role_attributes "
            "FROM pg_catalog.pg_user "
            "ORDER BY role_name desc ";

    QStringList list;

    list << "";

    int tuples;

    if (PQstatus(connection) == CONNECTION_OK) {
        PGresult *res = PQexec(connection, sql);

        if (PQresultStatus(res) != PGRES_TUPLES_OK)
        {
            PQclear(res);
        } else {

            tuples = PQntuples(res);

            for (int i = 0; i < tuples; i++)
                list << QString::fromStdString(PQgetvalue(res, i, 0));

            PQclear(res);

            return list;

        }
    }

    return QStringList();
}

bool ParameterBase::executeSQL(const char *sql, const char *params[], int num_params)
{
    if (!PQstatus(connection) == CONNECTION_OK)
        return false;

    query_result =  PQexecParams(connection, sql, num_params, NULL, params, NULL, NULL, 0);

    switch(PQresultStatus(query_result)) {
    case PGRES_EMPTY_QUERY:
        QMessageBox::critical(nullptr, "Error", "Empty Query", QMessageBox::Ok);
        break;
    case PGRES_TUPLES_OK:
        tuples = PQntuples(query_result);
        return true;
        break;
    case PGRES_COMMAND_OK:
    case PGRES_SINGLE_TUPLE:
    case PGRES_COPY_IN:
    case PGRES_COPY_OUT:
    case PGRES_COPY_BOTH:
    case PGRES_BAD_RESPONSE:
    case PGRES_FATAL_ERROR:
    case PGRES_NONFATAL_ERROR:
        QMessageBox::critical(nullptr, "Error", "Unexpected Query Result");
    }

    tuples = 0;
    PQclear(query_result);
    return false;

}

void ParameterBase::clearResult()
{
    PQclear(query_result);
}
