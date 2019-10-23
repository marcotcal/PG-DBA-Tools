#include "queryexecutor.h"

QueryExecutor::QueryExecutor(PGconn *conn, QString query, QObject *parent) :
    QThread(parent),
    conn(conn),
    query(query)
{

}

void QueryExecutor::run()
{
    PGresult *res = PQexec(conn, query.toStdString().c_str());
    emit query_ended(res);
}

