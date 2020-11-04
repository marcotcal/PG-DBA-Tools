#ifndef PGDBAGENERATORS_H
#define PGDBAGENERATORS_H

#include "pgdba_generators_global.h"
#include <QString>
#include <libpq-fe.h>


class PGDBAGenerators
{

public:
    PGDBAGenerators();

    QString getInsert(int offset, PGconn *connection);
    void setOnlyMandatory(bool value);

private:

    bool mandatory;

};

#endif // PGDBAGENERATORS_H
