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
    PGnotify *notify;

    PQsetNoticeProcessor(conn, noticeProcessor, NULL);
    PGresult *res = PQexec(conn, query.toStdString().c_str());
    PQconsumeInput(conn);
    while ((notify = PQnotifies(conn)) != NULL)
    {
        emit generate_listener_message(notify->relname, notify->be_pid, notify->extra);

        PQfreemem(notify);
        PQconsumeInput(conn);
    }

    emit query_ended(res);
}

void QueryExecutor::noticeProcessor(void *arg, const char *message)
{
    Q_UNUSED(arg)
    QString msg = QString(message);
    if (instance)
        emit instance->generate_notice(msg);
}
