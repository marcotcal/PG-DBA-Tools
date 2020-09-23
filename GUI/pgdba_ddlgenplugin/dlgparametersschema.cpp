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

void DlgParametersSchema::setOffset(int value)
{
    offset = value;
}

void DlgParametersSchema::setUserList(QStringList values)
{
    ui->schema_owner->addItems(values);
}

void DlgParametersSchema::setSchemas(QStringList values)
{
     ui->schema_name->addItems(values);
}

QString DlgParametersSchema::gen_create_schema()
{
    return create_schemas.join("\n");
}

QString DlgParametersSchema::gen_drop_schema()
{
    return drop_schemas.join("\n");
}

void DlgParametersSchema::on_schema_owner_currentIndexChanged(const QString &arg1)
{
    readSchemas();
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

            names.clear();
            create_schemas.clear();
            drop_schemas.clear();
            ui->schema_name->clear();

            names << "";

            for (int i = 0; i < tuples; i++) {

                name = QString::fromStdString(PQgetvalue(res, i, 0));
                owner = QString::fromStdString(PQgetvalue(res, i, 1));

                names << name;

                create_schemas  << (i > 0 ? QString(" ").repeated(offset) : "") + "CREATE SCHEMA " + name +
                                   " AUTHORIZATION " + owner + ";";
                drop_schemas  << (i > 0 ? QString(" ").repeated(offset) : "") + "DROP SCHEMA " + name +
                                       " AUTHORIZATION " + owner + ";";
            }

            ui->schema_name->addItems(names);

            PQclear(res);
        }
    }
}
