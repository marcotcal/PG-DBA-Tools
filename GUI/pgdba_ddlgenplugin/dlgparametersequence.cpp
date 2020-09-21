#include "dlgparametersequence.h"
#include "ui_dlgparametersequence.h"

DlgParameterSequence::DlgParameterSequence(PGconn *connection, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgParameterSequence),
    connection(connection)
{
    ui->setupUi(this);
    objectsList();
}

DlgParameterSequence::~DlgParameterSequence()
{
    delete ui;
}

QString DlgParameterSequence::schemaName()
{
    return ui->schema_name->currentText();
}

QString DlgParameterSequence::objectOwner()
{
    return ui->object_owner->currentText();
}

void DlgParameterSequence::setUserList(QStringList values)
{
    ui->object_owner->addItems(values);
}

void DlgParameterSequence::setSchemas(QStringList values)
{
    ui->schema_name->addItems(values);
}

void DlgParameterSequence::setObjecs(QStringList values)
{
    ui->object_name->addItems(values);
}

void DlgParameterSequence::objectsList()
{
    /* it will work only after version 10
    QString sql =
        "SELECT sequencename "
        "FROM pg_catalog.pg_sequences ";
    */

    QString sql =
        "SELECT relname, u.usename, n.nspname "
        "FROM pg_class c, "
        "     pg_user u, "
        "     pg_namespace n "
        "WHERE "
        "    c.relowner = u.usesysid "
        "    AND c.relkind = 'S' "
        "    AND n.oid = relnamespace "
        "    AND relnamespace IN "
        "        ( "
        "            SELECT oid "
        "                FROM pg_namespace "
        "             WHERE "
        "                nspname NOT LIKE 'pg_%' "
        "                AND nspname != 'information_schema' "
        "        ) ";

    QStringList list;
    QString schema_name;
    QString object_owner;

    schema_name = schemaName();
    object_owner = objectOwner();

    ui->object_name->clear();

    if (!schema_name.isEmpty()) {
        sql += QString("AND n.nspname = '%1' ").arg(schema_name);
    }

    if (!object_owner.isEmpty()) {
        sql += QString("AND u.usename = '%1' ").arg(object_owner);
    }

    list << "";

    int tuples;

    if (PQstatus(connection) == CONNECTION_OK) {

        PGresult *res =  PQexec(connection, sql.toStdString().c_str());

        if (PQresultStatus(res) != PGRES_TUPLES_OK)
        {
            PQclear(res);
        } else {

            tuples = PQntuples(res);

            for (int i = 0; i < tuples; i++)
                list << QString::fromStdString(PQgetvalue(res, i, 0));

            PQclear(res);

            ui->object_name->addItems(list);
        }
    }

    return;
}

void DlgParameterSequence::on_schema_name_currentIndexChanged(int index)
{
    objectsList();
}

void DlgParameterSequence::on_object_owner_currentIndexChanged(int index)
{
    objectsList();
}

QString DlgParameterSequence::gen_reset_sequece(PGconn *connection, int offset)
{
    /* This query will work only with versions greater than 10
    QString sql =
            "SELECT 'ALTER SEQUENCE ' || schemaname || '.' || sequencename || E' RESTART;\n' "
            "FROM pg_catalog.pg_sequences ";
    */

    QString sql =
        "SELECT 'ALTER SEQUENCE ' || n.nspname || '.' || relname || E' RESTART;\n' "
        "FROM pg_class c, "
        "     pg_user u, "
        "     pg_namespace n "
        "WHERE "
        "    c.relowner = u.usesysid "
        "    AND c.relkind = 'S' "
        "    AND n.oid = relnamespace "
        "    AND relnamespace IN "
        "        ( "
        "            SELECT oid "
        "                FROM pg_namespace "
        "             WHERE "
        "                nspname NOT LIKE 'pg_%' "
        "                AND nspname != 'information_schema' "
        "        ) ";
    QString result = "";
    int tuples;

    dlg.setLabel("Sequence");

    if (!dlg.exec())
        return "";

    schema_name = dlg.schemaName();
    object_owner = dlg.objectOwner();

    if (!schema_name.isEmpty())
        sql += QString("AND n.nspname ILIKE '%%1%'").arg(schema_name);

    if (!object_owner.isEmpty())
        sql += QString("AND u.usename ILIKE '%%1%'").arg(object_owner);

    if (PQstatus(connection) == CONNECTION_OK) {

        PGresult *res =  PQexec(connection, sql.toStdString().c_str());

        if (PQresultStatus(res) != PGRES_TUPLES_OK)
        {

            error = PQerrorMessage(connection);
            PQclear(res);

        } else {

            tuples = PQntuples(res);

            for (int i = 0; i < tuples; i++) {
                if (i > 0)
                    result += QString(" ").repeated(offset);
                result += QString::fromStdString(PQgetvalue(res, i, 0));
            }

            PQclear(res);
        }
    }

    return result;

}

QString DlgParameterSequence::gen_update_sequece(PGconn *connection, int offset)
{

}

