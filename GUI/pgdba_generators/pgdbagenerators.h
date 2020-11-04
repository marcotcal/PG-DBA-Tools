#ifndef PGDBAGENERATORS_H
#define PGDBAGENERATORS_H

#include "pgdba_generators_global.h"
#include <QString>
#include <libpq-fe.h>


class PGDBAGenerators
{

public:
    PGDBAGenerators(PGconn *connection);

    QString getInsert(int offset, PGconn *connection);
    void setOnlyMandatory(bool value);

    QStringList schemas();
    QStringList users();


private:

    bool mandatory;
    char *error;
    PGconn *connection;

};

#endif // PGDBAGENERATORS_H
