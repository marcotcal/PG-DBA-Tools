#include "sqlgenerationplugin.h"
#include <dlgparametersschema.h>
#include <dlgparameterobject.h>
#include <dlginsert.h>


SQLGenerationPlugin::SQLGenerationPlugin(QObject *parent) :
    QObject(parent)
{

}

void SQLGenerationPlugin::createTree(PGconn *connection, QTreeWidget *tree)
{
    QTreeWidgetItem *root = new QTreeWidgetItem();
    QTreeWidgetItem *schema_node;
    QTreeWidgetItem *schema;
    QStringList schemas_list;

    trees[tree] = connection;

    connect(tree, SIGNAL(itemActivated(QTreeWidgetItem*,int)), this, SLOT(processItem(QTreeWidgetItem*,int)));
    connect(tree, SIGNAL(itemClicked(QTreeWidgetItem*,int)), this, SLOT(processItem(QTreeWidgetItem*,int)));

    if (connection) {

        schemas_list = schemas(connection);
        root->setText(0, QString(PQdb(connection)));
        root->setIcon(0, QIcon(":/icons/images/icons/database.png"));
        tree->insertTopLevelItem(0, root);

        schema_node = new QTreeWidgetItem();
        schema_node->setText(0, "Schemas");
        schema_node->setIcon(0, QIcon(":/icons/images/icons/schemas.png"));
        schema_node->setData(0, ROLE_ITEM_TYPE, SCHEMAS_ITEM);
        root->addChild(schema_node);

        for(int i=0; i < schemas_list.count(); i++) {

            schema = new QTreeWidgetItem();
            schema->setText(0, schemas_list[i]);
            schema->setIcon(0, QIcon(":/icons/images/icons/schema.png"));
            schema->setData(0, ROLE_ITEM_TYPE, SCHEMA_ITEM);
            schema_node->addChild(schema);

        }

    }
}

QStringList SQLGenerationPlugin::run(PGconn *connection, int item)
{
    QStringList resp;

    switch(item) {
    case SQL_TEST:
        resp.append("-- Plugin Test.\n");
        resp.append("SELECT 'TESTING SQL GENERATION PLUGIN'\n");
        resp.append("-- End.\n");
        break;
    case SQL_INSERT_ALL:        
        //resp.insertAt(gen_insert_all(connection, index), line, index);
        break;
    case SQL_INSERT_MANDATORY:        
        //resp.insertAt(gen_insert_mandatory(connection, index), line, index);
        break;

    default:
        return QStringList();
    }

    return resp;

}

void SQLGenerationPlugin::updateFunctionList(QTreeWidgetItem *item, QListWidget *list)
{

    QListWidgetItem *list_item;
    int item_type;

    item_type = item->data(0, ROLE_ITEM_TYPE).toInt();

    list->clear();

    switch(item_type) {
    case TABLE_ITEM:
        list_item = new QListWidgetItem("Select first 100 rows");
        list_item->setData(ROLE_ITEM_TYPE, SQL_SELECT_FIRST_100);
        list->addItem(list_item);

        list_item = new QListWidgetItem("Select all rows");
        list_item->setData(ROLE_ITEM_TYPE, SQL_SELECT_ALL);
        list->addItem(list_item);

        list_item = new QListWidgetItem("Insert only mandatory fields");
        list_item->setData(ROLE_ITEM_TYPE, SQL_INSERT_MANDATORY);
        list->addItem(list_item);

        list_item = new QListWidgetItem("Insert all fields");
        list_item->setData(ROLE_ITEM_TYPE, SQL_INSERT_ALL);
        list->addItem(list_item);

        break;

    case VIEW_ITEM:
        list_item = new QListWidgetItem("Select first 100 rows");
        list_item->setData(ROLE_ITEM_TYPE, SQL_SELECT_FIRST_100);
        list->addItem(list_item);

        list_item = new QListWidgetItem("Select all rows");
        list_item->setData(ROLE_ITEM_TYPE, SQL_SELECT_ALL);
        list->addItem(list_item);

        list_item = new QListWidgetItem("Insert all fields");
        list_item->setData(ROLE_ITEM_TYPE, SQL_INSERT_ALL);
        list->addItem(list_item);

        break;

    case FUNCTION_ITEM:
        list_item = new QListWidgetItem("Select first 100 rows");
        list_item->setData(ROLE_ITEM_TYPE, SQL_SELECT_FIRST_100);
        list->addItem(list_item);

        list_item = new QListWidgetItem("Select all rows");
        list_item->setData(ROLE_ITEM_TYPE, SQL_SELECT_ALL);
        list->addItem(list_item);

        break;

    }

}

void SQLGenerationPlugin::processItem(QTreeWidgetItem *item, int column)
{
    switch(item->data(0, ROLE_ITEM_TYPE).toInt()) {
    case SCHEMA_ITEM:
        if(item->childCount() == 0) processSchemas(item);
        break;
    case TABLES_ITEM:
        if(item->childCount() == 0) processTables(item);
        break;
    case VIEWS_ITEM:
        if(item->childCount() == 0) processViews(item);
        break;
    case FUNCTIONS_ITEM:
        if(item->childCount() == 0) processFunctions(item);
        break;
    }
}

QStringList SQLGenerationPlugin::createObjectList(PGconn *connection, const char *sql, int return_col, int param_count, ...)
{
    QStringList list;
    int tuples;
    const char *params[param_count];
    PGresult *res;
    va_list vl;
    va_start(vl, param_count);

    for (int i = 0; i < param_count; i++) {
        params[i] = va_arg(vl, char *);
    }
    va_end(vl);

    if (PQstatus(connection) == CONNECTION_OK) {


        if (param_count > 0)
            res =  PQexecParams(connection, sql, param_count, NULL, params, NULL, NULL, 0);
        else
            res = PQexec(connection, sql);

        if (PQresultStatus(res) != PGRES_TUPLES_OK)
        {
            PQclear(res);
        } else {

            tuples = PQntuples(res);

            for (int i = 0; i < tuples; i++)
                list << QString::fromStdString(PQgetvalue(res, i, return_col));

            PQclear(res);

            return list;

        }
    }

    return QStringList();
}

void SQLGenerationPlugin::processSchemas(QTreeWidgetItem *item) {


    QTreeWidgetItem *table_node;
    QTreeWidgetItem *view_node;
    QTreeWidgetItem *function_node;


    if (item->childCount() > 0)
        return;

    function_node = new QTreeWidgetItem();
    function_node->setText(0, "Functions");
    function_node->setIcon(0, QIcon(":/icons/images/icons/functions.png"));
    function_node->setData(0, ROLE_ITEM_TYPE, FUNCTIONS_ITEM);
    function_node->setData(0, ROLE_SCHEMA_NAME, item->text(0));
    item->addChild(function_node);

    table_node = new QTreeWidgetItem();
    table_node->setText(0, "Tables");
    table_node->setIcon(0, QIcon(":/icons/images/icons/tables.png"));
    table_node->setData(0, ROLE_ITEM_TYPE, TABLES_ITEM);
    table_node->setData(0, ROLE_SCHEMA_NAME, item->text(0));
    item->addChild(table_node);

    view_node = new QTreeWidgetItem();
    view_node->setText(0, "Views");
    view_node->setIcon(0, QIcon(":/icons/images/icons/views.png"));
    view_node->setData(0, ROLE_ITEM_TYPE, VIEWS_ITEM);
    view_node->setData(0, ROLE_SCHEMA_NAME, item->text(0));
    item->addChild(view_node);

}

void SQLGenerationPlugin::processTables(QTreeWidgetItem *item)
{
    QTreeWidgetItem *table;
    QStringList table_list;

    PGconn *connection = trees[item->treeWidget()];

    table_list = tables(connection, item->data(0, ROLE_SCHEMA_NAME).toString());

    for(int j=0; j < table_list.count(); j++) {

        table = new QTreeWidgetItem();
        table->setText(0, table_list[j]);
        table->setIcon(0, QIcon(":/icons/images/icons/table.png"));
        table->setData(0, Qt::UserRole, TABLE_ITEM);
        item->addChild(table);

    }
}

void SQLGenerationPlugin::processViews(QTreeWidgetItem *item)
{
    QTreeWidgetItem *view;
    QStringList view_list;

    PGconn *connection = trees[item->treeWidget()];

    view_list = views(connection, item->data(0, ROLE_SCHEMA_NAME).toString());

    for(int j=0; j < view_list.count(); j++) {

        view = new QTreeWidgetItem();
        view->setText(0, view_list[j]);
        view->setIcon(0, QIcon(":/icons/images/icons/view.png"));
        view->setData(0, Qt::UserRole, VIEW_ITEM);
        item->addChild(view);

    }
}

void SQLGenerationPlugin::processFunctions(QTreeWidgetItem *item)
{
    QStringList function_list;
    QTreeWidgetItem *function;

    PGconn *connection = trees[item->treeWidget()];

    function_list = functions(connection, item->data(0, ROLE_SCHEMA_NAME).toString());

    for(int j=0; j < function_list.count(); j++) {

        function = new QTreeWidgetItem();
        function->setText(0, function_list[j]);
        function->setIcon(0, QIcon(":/icons/images/icons/function.png"));
        function->setData(0, ROLE_ITEM_TYPE, FUNCTION_ITEM);
        item->addChild(function);

    }
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
    return createObjectList(connection, sql, 0, 0);
}

QStringList SQLGenerationPlugin::schemas(PGconn *connection)
{
    const char *sql =
        "SELECT schema_name "
        "FROM information_schema.schemata "
        "WHERE schema_name NOT IN ('information_schema') AND schema_name !~ '^pg_' ";
    return createObjectList(connection, sql, 0, 0);
}

QStringList SQLGenerationPlugin::tables(PGconn *connection, QString schema)
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
    return createObjectList(connection, sql, 1, 1, schema.toStdString().c_str());
}

QStringList SQLGenerationPlugin::views(PGconn *connection, QString schema)
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
    return createObjectList(connection, sql, 1, 1, schema.toStdString().c_str());
}

QStringList SQLGenerationPlugin::functions(PGconn *connection, QString schema)
{
    const char *sql =
        "SELECT "
        "    n.nspname, "
        "    p.proname, "
        "    pg_catalog.pg_get_function_identity_arguments(p.oid) as arguments, "
        "    t.typname "
        "FROM   pg_catalog.pg_proc p "
        "       JOIN pg_catalog.pg_namespace n ON n.oid = p.pronamespace "
        "       JOIN pg_catalog.pg_type t ON p.prorettype = t.oid "
        "WHERE "
        "    t.typname != 'trigger' AND "
        "    n.nspname = $1 ";
    return createObjectList(connection, sql, 1, 1, schema.toStdString().c_str());
}

QString SQLGenerationPlugin::gen_insert_all(PGconn *connection, int offset)
{
    QStringList columns;
        QStringList values;
        QString column_name;
        QString column_default;
        QString data_type;
        QString is_mandatory;
        int tuples;
        QString offset_space = QString(" ").repeated(offset);
        char *error;
        const char *params[2];
        QString insert_stmt;
        const char *sql_list_fields =
            "SELECT column_name, column_default, data_type, is_nullable "
            "FROM information_schema.columns "
            "WHERE "
            "   table_schema = $1 "
            "   AND table_name = $2 ";

        DlgInsert dlg(connection);

        if (dlg.exec()) {

            insert_stmt = QString("INSERT INTO %1.%2 ( \n").arg(dlg.schema()).arg(dlg.table());

            params[0] = dlg.schema().toStdString().c_str();
            params[1] = dlg.table().toStdString().c_str();

            if (PQstatus(connection) == CONNECTION_OK) {

                PGresult *res =  PQexecParams(connection, sql_list_fields, 2, NULL, params, NULL, NULL, 0);

                if (PQresultStatus(res) != PGRES_TUPLES_OK)
                {

                    error = PQerrorMessage(connection);
                    PQclear(res);

                } else {

                    tuples = PQntuples(res);

                    for (int i = 0; i < tuples; i++) {

                        column_name = QString::fromStdString(PQgetvalue(res, i, 0));
                        column_default = QString::fromStdString(PQgetvalue(res, i, 1));
                        data_type = QString::fromStdString(PQgetvalue(res, i, 2));
                        is_mandatory = QString::fromStdString(PQgetvalue(res, i, 3));
                        //if (mandatory && is_mandatory == "NO")
                        //    continue;
                        if (column_default != "")
                            values << offset_space + "   " + column_default + "  /* " + column_name + " */";
                        else if (data_type == "character varying")
                            values << offset_space + "   '' /* " + column_name + " */";
                        else
                            values << offset_space + "      /* " + column_name + " */";
                        columns << offset_space + "   " + column_name;

                    }

                    PQclear(res);
                }
            }

        }

        insert_stmt += columns.join(QByteArray(",\n"));
        insert_stmt += "\n" + offset_space + ") VALUES (\n";
        insert_stmt += values.join(QByteArray(",\n"));
        insert_stmt += "\n" + offset_space + ");";

        return insert_stmt;
}

QString SQLGenerationPlugin::gen_insert_mandatory(PGconn *connection, int offset)
{


}

#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2(pgdba_sqlgenplugin, SQLGenerationPlugin)
#endif // QT_VERSION < 0x050000
