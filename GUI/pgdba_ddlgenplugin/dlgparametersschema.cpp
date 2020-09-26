#include "dlgparametersschema.h"
#include "ui_dlgparametersschema.h"

DlgParametersSchema::DlgParametersSchema(PGconn *connection, EditorItem *editor, QWidget *parent) :
    QDialog(parent),
    ParameterBase(connection, editor),
    ui(new Ui::DlgParametersSchema)
{
    ui->setupUi(this);
    ui->schema_owner->addItems(users());
}

DlgParametersSchema::~DlgParametersSchema()
{
    delete ui;
}

QString DlgParametersSchema::gen_create_schema()
{
    QString create_schemas = "";
    const char *params[2];
    const char *sql =
            "SELECT "
            "    'CREATE SCHEMA IF NOT EXISTS ' || schema_name || ' AUTHORIZATION ' || schema_owner || E';\n' AS create_schema "
            "FROM information_schema.schemata "
            "WHERE schema_name NOT IN ('public', 'information_schema') AND schema_name !~ '^pg_' "
            "   AND schema_name ILIKE $1 || '%' AND schema_owner ILIKE $2 || '%'  ";

    params[0] = ui->schema_name->currentText().toStdString().c_str();
    params[1] = ui->schema_owner->currentText().toStdString().c_str();

    executeSQL(sql, params, 2);

    for (int i = 0; i < tuples; i++) {
        create_schemas += (i > 0 ? QString(" ").repeated(index) : "") +
                QString::fromStdString(PQgetvalue(query_result, i, 0));
    }

    clearResult();

    return create_schemas;
}

QString DlgParametersSchema::gen_drop_schema()
{
    QString drop_schemas = "";
    const char *params[2];
    const char *sql =
            "SELECT "
            "    'DROP SCHEMA IF EXISTS ' || schema_name || E';\n' AS create_schema "
            "FROM information_schema.schemata "
            "WHERE schema_name NOT IN ('public', 'information_schema') AND schema_name !~ '^pg_' "
            "   AND schema_name ILIKE $1 || '%' AND schema_owner ILIKE $2 || '%'  ";

    params[0] = ui->schema_name->currentText().toStdString().c_str();
    params[1] = ui->schema_owner->currentText().toStdString().c_str();

    executeSQL(sql, params, 2);

    for (int i = 0; i < tuples; i++) {
        drop_schemas += (i > 0 ? QString(" ").repeated(index) : "") +
                QString::fromStdString(PQgetvalue(query_result, i, 0));
    }

    clearResult();

    return drop_schemas;
}

void DlgParametersSchema::on_schema_owner_currentIndexChanged(const QString &arg1)
{
    ui->schema_name->clear();
    ui->schema_name->addItems(schemas(arg1));
}
