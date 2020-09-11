#include "ddlgenerationplugin.h"
#include <dlgparametersschema.h>

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
    case DDL_CREATE_TRIGGER:
    case DDL_DROP_TRIGGER:
    case DDL_ENABLE_TRIGGER:
    case DDL_DISABLE_TRIGGER:
    default:
        return false;
    }

    return true;

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

    if (!dlg.exec())
        return "";

    schema_name = dlg.schemaName();
    schema_owner = dlg.schemaOwner();

    if (!schema_name.isNull())
        sql += QString("AND schema_name ILIKE '%%1%'").arg(schema_name);

    if (!schema_owner.isNull())
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
    int tuples;

    if (PQstatus(connection) == CONNECTION_OK) {

        //PGresult *res =  PQexecParams(connection, sql_list_fields, 2, NULL, params, NULL, NULL, 0);

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
Q_EXPORT_PLUGIN2(pgdba_ddlgenplugin, DDLGenerationPlugin)
#endif // QT_VERSION < 0x050000
