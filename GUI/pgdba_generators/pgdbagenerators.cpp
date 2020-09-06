#include "pgdbagenerators.h"
#include "dlginsert.h"


PGDBAGenerators::PGDBAGenerators()
{
    mandatory = false;
}

QString PGDBAGenerators::getInsert(PGconn *connection)
{

    DlgInsert dlg(connection);

    dlg.exec();

    return "VER 1.0";
}

void PGDBAGenerators::setOnlyMandatory(bool value)
{
    mandatory = value;
}
