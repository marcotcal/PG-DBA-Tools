#include "dlgparametersschema.h"
#include "ui_dlgparametersschema.h"

DlgParametersSchema::DlgParametersSchema(PGconn *connection, EditorItem *editor, QWidget *parent) :
    QDialog(parent),
    ParameterBase(connection, editor),
    ui(new Ui::DlgParametersSchema)
{
    ui->setupUi(this);
    ui->schema_owner->addItems(users());
    //ui->schema_name->addItems(schemas(""));
}

DlgParametersSchema::~DlgParametersSchema()
{
    delete ui;
}

QString DlgParametersSchema::gen_create_schema()
{
    // test code
    const char *params[10];
    const char *sql =
            "SELECT schema_name, schema_owner "
            "FROM information_schema.schemata "
            "WHERE schema_name NOT IN ('public', 'information_schema') AND schema_name !~ '^pg_' ";

    executeSQL(sql, nullptr, 0);

    return create_schemas.join("\n");
}

QString DlgParametersSchema::gen_drop_schema()
{
    return drop_schemas.join("\n");
}

void DlgParametersSchema::on_schema_owner_currentIndexChanged(const QString &arg1)
{
    ui->schema_name->clear();
    ui->schema_name->addItems(schemas(arg1));
}

void DlgParametersSchema::readSchemas()
{
    QString sql =
            "SELECT schema_name, schema_owner "
            "FROM information_schema.schemata "
            "WHERE schema_name NOT IN ('public', 'information_schema') AND schema_name !~ '^pg_' ";
    int tuples;
    QString name;
    QString owner;

    if (!ui->schema_owner->currentText().isEmpty())
        sql += QString("AND schema_owner = '%1'").arg(ui->schema_owner->currentText());

    if (!ui->schema_name->currentText().isEmpty())
        sql += QString("AND schema_name ILIKE '%%1%'").arg(ui->schema_name->currentText());

    if (PQstatus(connection) == CONNECTION_OK) {

        PGresult *res =  PQexec(connection, sql.toStdString().c_str());

        if (PQresultStatus(res) != PGRES_TUPLES_OK)
        {

            error = PQerrorMessage(connection);
            PQclear(res);

        } else {

            tuples = PQntuples(res);

            create_schemas.clear();
            drop_schemas.clear();

            for (int i = 0; i < tuples; i++) {

                name = QString::fromStdString(PQgetvalue(res, i, 0));
                owner = QString::fromStdString(PQgetvalue(res, i, 1));

                create_schemas  << (i > 0 ? QString(" ").repeated(offset) : "") + "CREATE SCHEMA " + name +
                                   " AUTHORIZATION " + owner + ";";
                drop_schemas  << (i > 0 ? QString(" ").repeated(offset) : "") + "DROP SCHEMA " + name +
                                       " AUTHORIZATION " + owner + ";";
            }

            PQclear(res);
        }
    }
}

void DlgParametersSchema::readRows()
{
    QString schema;
    QString owner;
    for (int i = 0; i < tuples; i++) {
        schema = QString::fromStdString(PQgetvalue(resp, i, 0));
        owner = QString::fromStdString(PQgetvalue(resp, i, 1));
    }

}
