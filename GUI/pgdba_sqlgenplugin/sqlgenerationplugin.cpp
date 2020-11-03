#include "sqlgenerationplugin.h"
#include <dlgparametersschema.h>
#include <dlgparameterobject.h>


SQLGenerationPlugin::SQLGenerationPlugin(QObject *parent) :
    QObject(parent)
{
}

void SQLGenerationPlugin::setMenu(QMenu *menu)
{

}

void SQLGenerationPlugin::setTreeWidget(QTreeWidget *tree)
{
    QTreeWidgetItem* sql_insert = new QTreeWidgetItem();
    QTreeWidgetItem* sql_update = new QTreeWidgetItem();
    QTreeWidgetItem* sql_delete = new QTreeWidgetItem();
    QTreeWidgetItem* item;

    sql_insert->setText(0,"Insert");
    sql_update->setText(0,"Update");
    sql_delete->setText(0,"Delete");

    tree->insertTopLevelItem(0, schemas);
    tree->insertTopLevelItem(1, sequences);
    tree->insertTopLevelItem(2,triggers);

    item = new QTreeWidgetItem(QStringList() << "Insert (All fields)", SQL_INSERT_ALL);
    sql_insert->addChild(item);

    item = new QTreeWidgetItem(QStringList() << "Insert (Only mandatory", SQL_INSERT_MANDATORY);
    sql_insert->addChild(item);

}

bool SQLGenerationPlugin::run(PGconn *connection, int item, EditorItem *editor)
{
    int line, index;

    switch(item) {
    case SQL_TEST:
        editor->append("-- Plugin Test.\n");
        editor->append("SELECT 'TESTING PLUGIN'\n");
        editor->append("-- End.\n");
        break;
    case SQL_INSERT_ALL:
        editor->getCursorPosition(&line, &index);
        editor->insertAt(gen_insert_all(connection, index), line, index);
        break;
    case SQL_INSERT_MANDATORY:
        editor->getCursorPosition(&line, &index);
        editor->insertAt(gen_insert_mandatory(connection, index), line, index);
        break;

    default:
        return false;
    }

    return true;

}

QStringList SQLGenerationPlugin::schemas(PGconn *connection)
{
    const char *sql =
        "SELECT schema_name "
        "FROM information_schema.schemata "
        "WHERE schema_name NOT IN ('information_schema') AND schema_name !~ '^pg_' ";

    QStringList list;

    list << "";

    int tuples;

    if (PQstatus(connection) == CONNECTION_OK) {
        PGresult *res = PQexec(connection, sql);

        if (PQresultStatus(res) != PGRES_TUPLES_OK)
        {
            PQclear(res);
        } else {

            tuples = PQntuples(res);

            for (int i = 0; i < tuples; i++)
                list << QString::fromStdString(PQgetvalue(res, i, 0));

            PQclear(res);

            return list;

        }
    }

    return QStringList();
}

QStringList SQLGenerationPlugin::users(PGconn *connection)
{
    const char *sql =
            "SELECT usename AS role_name, "
            "  CASE "
            "     WHEN usesuper AND usecreatedb THEN "
            "       CAST('superuser, create database' AS pg_catalog.text) "
            "     WHEN usesuper THEN "
            "        CAST('superuser' AS pg_catalog.text) "
            "     WHEN usecreatedb THEN "
            "        CAST('create database' AS pg_catalog.text) "
            "     ELSE "
            "        CAST('' AS pg_catalog.text) "
            "  END role_attributes "
            "FROM pg_catalog.pg_user "
            "ORDER BY role_name desc ";

    QStringList list;

    list << "";

    int tuples;

    if (PQstatus(connection) == CONNECTION_OK) {
        PGresult *res = PQexec(connection, sql);

        if (PQresultStatus(res) != PGRES_TUPLES_OK)
        {
            PQclear(res);
        } else {

            tuples = PQntuples(res);

            for (int i = 0; i < tuples; i++)
                list << QString::fromStdString(PQgetvalue(res, i, 0));

            PQclear(res);

            return list;

        }
    }

    return QStringList();
}

QString SQLGenerationPlugin::gen_insert_all(PGconn *connection, int offset)
{
    DlgParametersSchema dlg(connection);
    QString schema_name;
    QString schema_owner;
    QString sql =
            "SELECT "
            "    'CREATE SCHEMA IF NOT EXISTS ' || schema_name || ' AUTHORIZATION ' || schema_owner || E';\n' AS create_schema "
            "FROM information_schema.schemata "
            "WHERE schema_name NOT IN ('public', 'information_schema') AND schema_name !~ '^pg_' ";
    QString result = "";
    int tuples;

    dlg.setSchemas(schemas(connection));
    dlg.setUserList(users(connection));

    if (!dlg.exec())
        return "";

    schema_name = dlg.schemaName();
    schema_owner = dlg.schemaOwner();

    if (!schema_name.isEmpty())
        sql += QString("AND schema_name ILIKE '%%1%'").arg(schema_name);

    if (!schema_owner.isEmpty())
        sql += QString("AND schema_owner ILIKE '%%1%'").arg(schema_owner);

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

QString SQLGenerationPlugin::gen_insert_mandatory(PGconn *connection, int offset)
{
    QString sql =
            "SELECT "
            "    'DROP SCHEMA IF EXISTS ' || schema_name || E';\n' AS drop_schema "
            "FROM information_schema.schemata "
            "WHERE schema_name NOT IN ('public', 'information_schema') AND schema_name !~ '^pg_' ";
    QString result = "";
    DlgParametersSchema dlg(connection);
    QString schema_name;
    QString schema_owner;
    int tuples;

    dlg.setSchemas(schemas(connection));
    dlg.setUserList(users(connection));
    if (!dlg.exec())
        return "";

    schema_name = dlg.schemaName();
    schema_owner = dlg.schemaOwner();

    if (!schema_name.isEmpty())
        sql += QString("AND schema_name ILIKE '%%1%'").arg(schema_name);

    if (!schema_owner.isEmpty())
        sql += QString("AND schema_owner ILIKE '%%1%'").arg(schema_owner);

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

#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2(pgdba_sqlgenplugin, SQLGenerationPlugin)
#endif // QT_VERSION < 0x050000
