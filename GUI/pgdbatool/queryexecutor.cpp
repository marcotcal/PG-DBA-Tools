#include "queryexecutor.h"

QueryExecutor *QueryExecutor::instance = nullptr;

QueryExecutor::QueryExecutor(PGconn *conn, QString query, QObject *parent) :
    QThread(parent),
    conn(conn),
    query(query)
{
    instance = this;
}

void QueryExecutor::run()
{
    PQsetNoticeProcessor(conn, noticeProcessor, NULL);
    PGresult *res = PQexec(conn, query.toStdString().c_str());
    emit query_ended(res);
}

void QueryExecutor::noticeProcessor(void *arg, const char *message)
{
    Q_UNUSED(arg)
    QString msg = QString(message);
    if (instance)
        emit instance->generate_notice(msg);
}
