#include "ddlgenerationplugin.h"
#include <dlgparametersschema.h>
#include <dlgparameterobject.h>


DDLGenerationPlugin::DDLGenerationPlugin(QObject *parent) :
    QObject(parent)
{

}

void DDLGenerationPlugin::setMenu(QMenu *menu)
{

}

void DDLGenerationPlugin::setTreeWidget(QTreeWidget *tree)
{
    QTreeWidgetItem* schemas = new QTreeWidgetItem();
    QTreeWidgetItem* sequences = new QTreeWidgetItem();
    QTreeWidgetItem* triggers = new QTreeWidgetItem();
    QTreeWidgetItem* item;

    schemas->setText(0,"Schemas");
    sequences->setText(0,"Sequences");
    triggers->setText(0,"Triggers");

    tree->insertTopLevelItem(0, schemas);
    tree->insertTopLevelItem(1, sequences);
    tree->insertTopLevelItem(2,triggers);

    item = new QTreeWidgetItem(QStringList() << "Create Schema", DDL_CREATE_SCHEMA);
    schemas->addChild(item);

    item = new QTreeWidgetItem(QStringList() << "Drop Schema", DDL_DROP_SCHEMA);
    schemas->addChild(item);

    item = new QTreeWidgetItem(QStringList() << "Update Sequences", DDL_UPDATE_SEQUENCE);
    sequences->addChild(item);

    item = new QTreeWidgetItem(QStringList() << "Reset Sequences", DDL_RESET_SEQUENCE);
    sequences->addChild(item);

    item = new QTreeWidgetItem(QStringList() << "Create Trigger", DDL_CREATE_TRIGGER);
    triggers->addChild(item);

    item = new QTreeWidgetItem(QStringList() << "Drop Trigger", DDL_DROP_TRIGGER);
    triggers->addChild(item);

    item = new QTreeWidgetItem(QStringList() << "Enable Trigger", DDL_ENABLE_TRIGGER);
    triggers->addChild(item);

    item = new QTreeWidgetItem(QStringList() << "Disable Trigger", DDL_DISABLE_TRIGGER);
    triggers->addChild(item);

}

bool DDLGenerationPlugin::run(PGconn *connection, int item, EditorItem *editor)
{
    int line, index;
    PGDBAGenerators *generators = new PGDBAGenerators(connection);

    switch(item) {
    case DDL_TEST:
        editor->append("-- Plugin Test.\n");
        editor->append("SELECT 'TESTING PLUGIN'\n");
        editor->append("-- End.\n");
        break;
    case DDL_CREATE_SCHEMA:
        editor->getCursorPosition(&line, &index);
        editor->insertAt(gen_create_schema(connection, index), line, index);
        break;
    case DDL_DROP_SCHEMA:
        editor->getCursorPosition(&line, &index);
        editor->insertAt(gen_drop_schema(connection, index), line, index);
        break;
    case DDL_UPDATE_SEQUENCE:
    case DDL_RESET_SEQUENCE:
        editor->getCursorPosition(&line, &index);
        editor->insertAt(gen_reset_sequece(connection, index), line, index);
        break;
    case DDL_CREATE_TRIGGER:
    case DDL_DROP_TRIGGER:
    case DDL_ENABLE_TRIGGER:
    case DDL_DISABLE_TRIGGER:
    default:
        delete generators;
        return false;
    }

    delete generators;
    return true;

}

QStringList DDLGenerationPlugin::schemas(PGconn *connection)
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

QStringList DDLGenerationPlugin::users(PGconn *connection)
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

QString DDLGenerationPlugin::gen_create_schema(PGconn *connection, int offset)
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

QString DDLGenerationPlugin::gen_drop_schema(PGconn *connection, int offset)
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

QString DDLGenerationPlugin::gen_update_sequece(PGconn *connection, int offset)
{

}

QString DDLGenerationPlugin::gen_reset_sequece(PGconn *connection, int offset)
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
    DlgParameterObject dlg(connection);
    QString schema_name;
    QString object_owner;
    int tuples;

    dlg.setSchemas(schemas(connection));
    dlg.setUserList(users(connection));
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

QString DDLGenerationPlugin::gen_create_trigger(PGconn *connection, int offset)
{

}

QString DDLGenerationPlugin::gen_drop_trigger(PGconn *connection, int offset)
{

}

QString DDLGenerationPlugin::gen_disable_trigger(PGconn *connection, int offset)
{

}

QString DDLGenerationPlugin::gen_enable_trigger(PGconn *connection, int offset)
{

}


#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2(pgdba_ddlgenplugin, DDLGenerationPlugin)
#endif // QT_VERSION < 0x050000
