#include "dlgparameterobject.h"
#include "ui_dlgparameterobject.h"

DlgParameterObject::DlgParameterObject(PGconn *connection, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgParameterObject),
    connection(connection)
{
    ui->setupUi(this);
    objectsList();
}

DlgParameterObject::~DlgParameterObject()
{
    delete ui;
}

QString DlgParameterObject::schemaName()
{
    return ui->schema_name->currentText();
}

QString DlgParameterObject::objectOwner()
{
    return ui->object_owner->currentText();
}

QString DlgParameterObject::objectName()
{
    return ui->object_name->currentText();
}

void DlgParameterObject::setLabel(QString value)
{
    ui->object_label->setText(value);
}

void DlgParameterObject::setUserList(QStringList values)
{
    ui->object_owner->addItems(values);
}

void DlgParameterObject::setSchemas(QStringList values)
{
    ui->schema_name->addItems(values);
}

void DlgParameterObject::setObjecs(QStringList values)
{
    ui->object_name->addItems(values);
}

void DlgParameterObject::objectsList()
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

void DlgParameterObject::on_schema_name_currentIndexChanged(int index)
{
    objectsList();
}

void DlgParameterObject::on_object_owner_currentIndexChanged(int index)
{
    objectsList();
}
