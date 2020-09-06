#include "dlginsert.h"
#include "ui_dlginsert.h"

DlgInsert::DlgInsert(PGconn *connection, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgInsert),
    connection(connection)
{
    ui->setupUi(this);
    getSchemas();
}

DlgInsert::~DlgInsert()
{
    delete ui;
}

void DlgInsert::getSchemas()
{
    const char *sql =
        "SELECT schema_name "
        "FROM information_schema.schemata "
        "WHERE schema_name NOT IN ('information_schema') AND schema_name !~ '^pg_' ";

    QStringList list;
    int tuples;

    if (PQstatus(connection) == CONNECTION_OK) {
        PGresult *res = PQexec(connection, sql);

        if (PQresultStatus(res) != PGRES_TUPLES_OK)
        {
            PQclear(res);
            return;
        }

        tuples = PQntuples(res);

        for (int i = 0; i < tuples; i++)
            list << QString::fromStdString(PQgetvalue(res, i, 0));

        ui->schemas->clear();
        ui->schemas->addItems(list);

        PQclear(res);

    }

}

void DlgInsert::getTables(const char *schema)
{
    const char *sql =
            "SELECT "
            "   tablename "
            "FROM pg_catalog.pg_tables "
            "WHERE "
            "   schemaname NOT IN ('pg_catalog', 'information_schema') AND schemaname = $1 ";
    const char *params[1];
    QStringList list;
    int tuples;

    params[0] = schema;

    if (PQstatus(connection) == CONNECTION_OK) {
        PGresult *res = PQexecParams(connection, sql, 1, NULL, params, NULL, NULL, 0);

        if (PQresultStatus(res) != PGRES_TUPLES_OK)
        {
            PQclear(res);
            return;
        }

        tuples = PQntuples(res);

        for (int i = 0; i < tuples; i++)
            list << QString::fromStdString(PQgetvalue(res, i, 0));

        ui->tables->addItems(list);

        PQclear(res);

    }

}

void DlgInsert::getViews(const char *schema)
{
    const char *sql =
        "SELECT "
        "   viewname "
        "FROM pg_catalog.pg_views "
        "WHERE schemaname NOT IN ('pg_catalog', 'information_schema') AND schemaname = $1 ";
    const char *params[1];
    QStringList list;
    int tuples;

    params[0] = schema;

    if (PQstatus(connection) == CONNECTION_OK) {
        PGresult *res = PQexecParams(connection, sql, 1, NULL, params, NULL, NULL, 0);

        if (PQresultStatus(res) != PGRES_TUPLES_OK)
        {
            PQclear(res);
            return;
        }

        tuples = PQntuples(res);

        for (int i = 0; i < tuples; i++)
            list << QString::fromStdString(PQgetvalue(res, i, 0));

        ui->tables->addItems(list);

        PQclear(res);

    }

}

void DlgInsert::on_schemas_currentTextChanged(const QString &arg1)
{
    ui->tables->clear();
    if (ui->show_tables->isChecked())
        getTables(arg1.toStdString().c_str());
    if (ui->show_views->isChecked())
        getViews(arg1.toStdString().c_str());

}

void DlgInsert::on_show_views_clicked()
{
    ui->tables->clear();
    if (ui->show_tables->isChecked())
        getTables(ui->schemas->itemText(ui->schemas->currentIndex()).toStdString().c_str());
    if (ui->show_views->isChecked())
        getViews(ui->schemas->itemText(ui->schemas->currentIndex()).toStdString().c_str());
}

void DlgInsert::on_show_tables_clicked()
{
    ui->tables->clear();
    if (ui->show_tables->isChecked())
        getTables(ui->schemas->itemText(ui->schemas->currentIndex()).toStdString().c_str());
    if (ui->show_views->isChecked())
        getViews(ui->schemas->itemText(ui->schemas->currentIndex()).toStdString().c_str());
}
