#include "ddlgenerationplugin.h"
#include <dlgparametersschema.h>
#include <dlgparametersequence.h>


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

    DlgParametersSchema dlg_schema(connection);
    DlgParameterSequence dlg_sequence(connection);

    dlg_schema.setSchemas(schemas(connection));
    dlg_schema.setUserList(users(connection));

    dlg_sequence.setSchemas(schemas(connection));
    dlg_sequence.setUserList(users(connection));

    editor->getCursorPosition(&line, &index);

    dlg_schema.setOffset(index);

    switch(item) {
    case DDL_TEST:
        editor->append("-- Plugin Test.\n");
        editor->append("SELECT 'TESTING PLUGIN'\n");
        editor->append("-- End.\n");
        break;
    case DDL_CREATE_SCHEMA:
        if (dlg_schema.exec()) {
            editor->insertAt(dlg_schema.gen_create_schema(), line, index);
        }
        break;
    case DDL_DROP_SCHEMA:
        if (dlg_schema.exec()) {
            editor->insertAt(dlg_schema.gen_create_schema(), line, index);
        }
        break;
    case DDL_UPDATE_SEQUENCE:
    case DDL_RESET_SEQUENCE:
        editor->getCursorPosition(&line, &index);
        editor->insertAt(dlg_sequence.gen_reset_sequece(connection, index), line, index);
        break;
    case DDL_CREATE_TRIGGER:
    case DDL_DROP_TRIGGER:
    case DDL_ENABLE_TRIGGER:
    case DDL_DISABLE_TRIGGER:
    default:
        return false;
    }

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

#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2(pgdba_ddlgenplugin, DDLGenerationPlugin)
#endif // QT_VERSION < 0x050000
