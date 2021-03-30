#include "ddlgenerationplugin.h"


DDLGenerationPlugin::DDLGenerationPlugin(QObject *parent) :
    QObject(parent)
{

}

void DDLGenerationPlugin::setMenu(QMenu *menu)
{

}

void DDLGenerationPlugin::setTreeWidget(QTreeWidget *value)
{
    tree = value;
}

void DDLGenerationPlugin::createTree(PGconn *connection)
{
    QTreeWidgetItem *root = new QTreeWidgetItem();
    QTreeWidgetItem *schema_node;

    QTreeWidgetItem *table_node;
    QTreeWidgetItem *trigger_node;
    QTreeWidgetItem *index_node;

    QTreeWidgetItem *view_node;
    QTreeWidgetItem *schema;
    QTreeWidgetItem *table;
    QTreeWidgetItem *view;
    QStringList schemas_list;
    QStringList table_list;
    QStringList view_list;

    if (connection) {

        schemas_list = schemas(connection);
        root->setText(0, QString(PQdb(connection)));
        root->setIcon(0, QIcon(":/icons/images/icons/database.png"));
        tree->insertTopLevelItem(0, root);


        schema_node = new QTreeWidgetItem();
        schema_node->setText(0, "Schemas");
        schema_node->setIcon(0, QIcon(":/icons/images/icons/schemas.png"));
        root->addChild(schema_node);

        for(int i=0; i < schemas_list.count(); i++) {

            schema = new QTreeWidgetItem();
            schema->setText(0, schemas_list[i]);
            schema->setIcon(0, QIcon(":/icons/images/icons/schema.png"));
            schema_node->addChild(schema);

            table_node = new QTreeWidgetItem();
            table_node->setText(0, "Tables");
            table_node->setIcon(0, QIcon(":/icons/images/icons/tables.png"));
            schema->addChild(table_node);

            table_list = tables(schemas_list[i], connection);

            for(int j=0; j < table_list.count(); j++) {

                table = new QTreeWidgetItem();
                table->setText(0, table_list[j]);
                table->setIcon(0, QIcon(":/icons/images/icons/table.png"));
                table_node->addChild(table);

            }

            view_node = new QTreeWidgetItem();
            view_node->setText(0, "Views");
            view_node->setIcon(0, QIcon(":/icons/images/icons/views.png"));
            schema->addChild(view_node);

            view_list = views(schemas_list[i], connection);

            for(int j=0; j < view_list.count(); j++) {

                view = new QTreeWidgetItem();
                view->setText(0, view_list[j]);
                view->setIcon(0, QIcon(":/icons/images/icons/view.png"));
                view_node->addChild(view);

            }

        }

    }
}


bool DDLGenerationPlugin::run(PGconn *connection, int item, EditorItem *editor)
{

    /*
    int line, index;
    DlgParametersSchema *dlg_schema;
    DlgParameterSequence *dlg_sequence;

    if (connection) {
        generators = new PGDBAGenerators(connection);
        dlg_schema = new DlgParametersSchema(connection, editor);
        dlg_sequence = new DlgParameterSequence(connection, editor);
    }

    switch(item) {
    case DDL_TEST:
        editor->append("-- Plugin Test.\n");
        editor->append("SELECT 'TESTING DDL GENERATION PLUGIN'\n");
        editor->append("-- End.\n");
        break;
    case DDL_CREATE_SCHEMA:
        if (dlg_schema->exec()) {
            editor->insertAt(dlg_schema->gen_create_schema(), line, index);
        }
        break;
    case DDL_DROP_SCHEMA:
        if (dlg_schema->exec()) {
            editor->insertAt(dlg_schema->gen_drop_schema(), line, index);
        }
        break;
    case DDL_UPDATE_SEQUENCE:
        if (dlg_sequence->exec()) {
            editor->insertAt(dlg_sequence->gen_update_sequece(), line, index);
        }
        break;
    case DDL_RESET_SEQUENCE:
        if (dlg_sequence->exec()) {
            editor->insertAt(dlg_sequence->gen_reset_sequece(), line, index);
        }
        break;
    case DDL_CREATE_TRIGGER:
    case DDL_DROP_TRIGGER:
    case DDL_ENABLE_TRIGGER:
    case DDL_DISABLE_TRIGGER:
    default:
        if (connection) {
            delete generators;
            delete dlg_schema;
            delete dlg_sequence;
        }
        return false;
    }

    if (connection) {
        delete generators;
        delete dlg_schema;
        delete dlg_sequence;
    }
    return true;
    */
}

QStringList DDLGenerationPlugin::schemas(PGconn *connection)
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

QStringList DDLGenerationPlugin::tables(QString schema, PGconn *connection)
{
    const char *sql =
            "SELECT "
            "    schemaname, "
            "    tablename, "
            "    tableowner, "
            "    tablespace, "
            "    hasindexes, "
            "    hasrules, "
            "    hastriggers, "
            "    rowsecurity "
            "FROM "
            "    pg_catalog.pg_tables "
            "WHERE "
            "    schemaname != 'pg_catalog' "
            "    AND schemaname != 'information_schema' "
            "    AND schemaname = $1 ";
    const char *params[1];
    QStringList list;
    int tuples;

    params[0] = schema.toStdString().c_str();

    if (PQstatus(connection) == CONNECTION_OK) {

        PGresult *res =  PQexecParams(connection, sql, 1, NULL, params, NULL, NULL, 0);

        if (PQresultStatus(res) != PGRES_TUPLES_OK)
        {
            PQclear(res);
        } else {

            tuples = PQntuples(res);

            for (int i = 0; i < tuples; i++)
                list << QString::fromStdString(PQgetvalue(res, i, 1));

            PQclear(res);

            return list;

        }
    }

    return QStringList();
}

QStringList DDLGenerationPlugin::views(QString schema, PGconn *connection)
{

    const char *sql =
        "SELECT "
        "    schemaname, "
        "    viewname, "
        "    viewowner, "
        "    definition "
        "FROM pg_catalog.pg_views "
        "WHERE "
        "    schemaname NOT IN ('pg_catalog', 'information_schema') "
        "    AND schemaname = $1 ";
    const char *params[1];
    QStringList list;
    int tuples;

    params[0] = schema.toStdString().c_str();

    if (PQstatus(connection) == CONNECTION_OK) {

        PGresult *res =  PQexecParams(connection, sql, 1, NULL, params, NULL, NULL, 0);

        if (PQresultStatus(res) != PGRES_TUPLES_OK)
        {
            PQclear(res);
        } else {

            tuples = PQntuples(res);

            for (int i = 0; i < tuples; i++)
                list << QString::fromStdString(PQgetvalue(res, i, 1));

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
