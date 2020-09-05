#include "pgdbagenerators.h"
#include "dlginsert.h"


PGDBAGenerators::PGDBAGenerators()
{

}

QString PGDBAGenerators::getInsert(PGconn *connection)
{
    Q_UNUSED(connection)

    DlgInsert dlg;

    dlg.exec();

    return "VER 1.0";
}
