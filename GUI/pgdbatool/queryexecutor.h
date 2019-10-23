#ifndef QUERYEXECUTOR_H
#define QUERYEXECUTOR_H

#include <QObject>
#include <QThread>
#include "resultoutput.h"
#include "outputset.h"
#include <libpq-fe.h>

class QueryExecutor : public QThread
{

    Q_OBJECT

public:
    QueryExecutor(PGconn *conn, QString query, QObject *parent=nullptr);

protected:
    void run() override;

signals:
    void query_ended(PGresult *res);

private:
    PGconn *conn;
    QString query;
};

#endif // QUERYEXECUTOR_H
