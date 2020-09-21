#include "dlgparametersschema.h"
#include "ui_dlgparametersschema.h"

DlgParametersSchema::DlgParametersSchema(PGconn *connection, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgParametersSchema),
    connection(connection)
{
    ui->setupUi(this);
}

DlgParametersSchema::~DlgParametersSchema()
{
    delete ui;
}

void DlgParametersSchema::setUserList(QStringList values)
{
    ui->schema_owner->addItems(values);
}

void DlgParametersSchema::setSchemas(QStringList values)
{
     ui->schema_name->addItems(values);
}

QString DlgParametersSchema::gen_create_schema(PGconn *connection, int offset)
{
    QString sql =
            "SELECT "
            "    'CREATE SCHEMA IF NOT EXISTS ' || schema_name || ' AUTHORIZATION ' || schema_owner || E';\n' AS create_schema "
            "FROM information_schema.schemata "
            "WHERE schema_name NOT IN ('public', 'information_schema') AND schema_name !~ '^pg_' ";
    QString result = "";
    int tuples;

    if (!exec())
        return "";

    if (!ui->schema_name->currentText().isEmpty())
        sql += QString("AND schema_name ILIKE '%%1%'").arg(ui->schema_name->currentText());

    if (!ui->schema_owner->currentText().isEmpty())
        sql += QString("AND schema_owner ILIKE '%%1%'").arg(ui->schema_owner->currentText());

    if (PQstatus(connection) == CONNECTION_OK) {

        //PGresult *res =  PQexecParams(connection, sql_list_fields, 2, NULL, params, NULL, NULL, 0);

        PGresult *res =  PQexec(connection, sql.toStdString().c_str());

        if (PQresultStatus(res) != PGRES_TUPLES_OK)
        {

            error = PQerrorMessage(connection);
            PQclear(res);
            return error;

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

QString DlgParametersSchema::gen_drop_schema(PGconn *connection, int offset)
{
    QString sql =
            "SELECT "
            "    'DROP SCHEMA IF EXISTS ' || schema_name || E';\n' AS drop_schema "
            "FROM information_schema.schemata "
            "WHERE schema_name NOT IN ('public', 'information_schema') AND schema_name !~ '^pg_' ";
    QString result = "";

    int tuples;

    if (!exec())
        return "";

    if (!ui->schema_name->currentText().isEmpty())
        sql += QString("AND schema_name ILIKE '%%1%'").arg(ui->schema_name->currentText());

    if (!ui->schema_owner->currentText().isEmpty())
        sql += QString("AND schema_owner ILIKE '%%1%'").arg(ui->schema_owner->currentText());

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
