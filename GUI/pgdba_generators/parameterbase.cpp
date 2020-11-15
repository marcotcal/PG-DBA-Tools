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

QString ParameterBase::databaseVersion()
{
    const char* sql = "SELECT (regexp_matches(version(),'^PostgreSQL [0-9]*\.[0-9]*'))[1]";
    QString version = "";

    if(executeSQL(sql, nullptr, 0)) {
        version = QString::fromStdString(PQgetvalue(query_result, 0, 0));
        clearResult();
    }

    return version;
}

bool ParameterBase::executeSQL(const char *sql, const char *params[], int num_params)
{
    ExecStatusType status;

    // It is expected that all queries executed by this method return tuples

    if (!PQstatus(connection) == CONNECTION_OK)
        return false;

    // the below code will permit the executions of unamed functions (usings DO) and queries
    // with multiple sentences.
    // In these cases we cannot use parametes since multiple sentences queries cannot be prepared
    if (num_params == 0)
        query_result = PQexec(connection, sql);
    else
        query_result =  PQexecParams(connection, sql, num_params, NULL, params, NULL, NULL, 0);

    status = PQresultStatus(query_result);
    if (status == PGRES_EMPTY_QUERY) {
        QMessageBox::critical(nullptr, "Error", "Empty Query", QMessageBox::Ok);
    } else if (status == PGRES_TUPLES_OK) {
        tuples = PQntuples(query_result);
        return true;
    }

    QMessageBox::critical(nullptr, "Error", "Unexpected Query Result");
    tuples = 0;
    PQclear(query_result);
    return false;

}

void ParameterBase::clearResult()
{
    PQclear(query_result);
}
