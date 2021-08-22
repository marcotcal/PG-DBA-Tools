#include "sqlgenerationplugin.h"
#include <dlginsert.h>
#include <dlgupdate.h>


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
            schema->setData(0, ROLE_SCHEMA_NAME, schemas_list[i]);
            schema_node->addChild(schema);

        }

    }
}

QStringList SQLGenerationPlugin::run(QTreeWidgetItem *tree_item, PGconn *connection, int command)
{
    QStringList resp;
    QString schema_name = tree_item->data(0, ROLE_SCHEMA_NAME).toString();
    QString table_name = tree_item->data(0, ROLE_TABLE_NAME).toString();

    switch(command) {
    case SQL_TEST:
        resp.append("-- Plugin Test.\n");
        resp.append("SELECT 'TESTING SQL GENERATION PLUGIN'\n");
        resp.append("-- End.\n");
        break;
    case SQL_INSERT_IN_TABLE:
        resp = insert_record(connection, schema_name, table_name);
        break;
    case SQL_UPDATE_IN_TABLE:
        resp = update_record(connection, schema_name, table_name);
        break;
    case SQL_SELECT_TABLE:
        resp = select_in_table(connection, schema_name, table_name, -1);
        break;
    case SQL_SELECT_TABLE_FIRST_100:
        resp = select_in_table(connection, schema_name, table_name, 100);
        break;
    default:
        return QStringList();
    }

    return resp;

}

void SQLGenerationPlugin::updateFunctionList(QTreeWidgetItem* item,  QListWidget *list)
{

    QListWidgetItem *list_item;
    int item_type;

    item_type = item->data(0, ROLE_ITEM_TYPE).toInt();

    list->clear();

    switch(item_type) {
    case TABLE_ITEM:
        list_item = new QListWidgetItem("Select first 100 rows");
        list_item->setData(ROLE_ITEM_TYPE, SQL_SELECT_TABLE_FIRST_100);
        list->addItem(list_item);

        list_item = new QListWidgetItem("Select all rows");
        list_item->setData(ROLE_ITEM_TYPE, SQL_SELECT_TABLE);
        list->addItem(list_item);

        list_item = new QListWidgetItem("Insert record");
        list_item->setData(ROLE_ITEM_TYPE, SQL_INSERT_IN_TABLE);
        list->addItem(list_item);

        list_item = new QListWidgetItem("Udate record");
        list_item->setData(ROLE_ITEM_TYPE, SQL_UPDATE_IN_TABLE);
        list->addItem(list_item);

        break;

    case VIEW_ITEM:
        list_item = new QListWidgetItem("Select first 100 rows");
        list_item->setData(ROLE_ITEM_TYPE, SQL_SELECT_VIEW_FIRST_100);
        list->addItem(list_item);

        list_item = new QListWidgetItem("Select all rows");
        list_item->setData(ROLE_ITEM_TYPE, SQL_SELECT_VIEW);
        list->addItem(list_item);

        list_item = new QListWidgetItem("Insert record");
        list_item->setData(ROLE_ITEM_TYPE, SQL_INSERT_IN_TABLE);
        list->addItem(list_item);

        list_item = new QListWidgetItem("Udate record");
        list_item->setData(ROLE_ITEM_TYPE, SQL_UPDATE_IN_VIEW);
        list->addItem(list_item);

        break;

    case FUNCTION_ITEM:
        list_item = new QListWidgetItem("Select first 100 rows");
        list_item->setData(ROLE_ITEM_TYPE, SQL_SELECT_FUNCTION_FIRST_100);
        list->addItem(list_item);

        list_item = new QListWidgetItem("Select all rows");
        list_item->setData(ROLE_ITEM_TYPE, SQL_SELECT_FUNCTION);
        list->addItem(list_item);

        break;

    }

}

void SQLGenerationPlugin::processItem(QTreeWidgetItem *item, int column)
{
    Q_UNUSED(column)

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

PGresult *SQLGenerationPlugin::createObjectList(PGconn *connection, const char *sql, int param_count, ...)
{

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
            return res;
        }
    }

    return nullptr;
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
    QString schema = item->data(0, ROLE_SCHEMA_NAME).toString();

    PGconn *connection = trees[item->treeWidget()];

    table_list = tables(connection, schema);

    for(int j=0; j < table_list.count(); j++) {

        table = new QTreeWidgetItem();
        table->setText(0, table_list[j]);
        table->setIcon(0, QIcon(":/icons/images/icons/table.png"));
        table->setData(0, ROLE_ITEM_TYPE, TABLE_ITEM);
        table->setData(0, ROLE_TABLE_NAME, table_list[j]);
        table->setData(0, ROLE_SCHEMA_NAME, schema);
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

QStringList SQLGenerationPlugin::insert_record(PGconn *connection, QString schema, QString table)
{
        QString column_name;
        QString comment;
        QString column_default;
        QString data_type;
        QString is_mandatory;
        int tuples;        
        QStringList insert_stmt;
        DlgInsert dlg;

        const char *sql_list_fields =
            "SELECT column_name, column_default, data_type, is_nullable "
            "FROM information_schema.columns "
            "WHERE "
            "   table_schema = $1 "
            "   AND table_name = $2 ";

        if(dlg.exec()) {

            insert_stmt << QString("INSERT INTO %1.%2 ( \n").arg(schema).arg(table);

            PGresult *res = createObjectList(connection, sql_list_fields, 2, schema.toStdString().c_str(),
                                             table.toStdString().c_str());

            if (PQresultStatus(res) != PGRES_TUPLES_OK)
            {
                insert_stmt << PQerrorMessage(connection);
                PQclear(res);
            } else {

                tuples = PQntuples(res);

                for (int i = 0; i < tuples; i++) {
                    column_name = QString::fromStdString(PQgetvalue(res, i, 0));
                        insert_stmt <<  (i == 0 ? "    ":"   ,") + column_name + "\n";
                }

                insert_stmt << ") VALUES ( \n";

                for (int i = 0; i < tuples; i++) {
                    column_name = QString::fromStdString(PQgetvalue(res, i, 0));
                    data_type = QString::fromStdString(PQgetvalue(res, i, 2));
                    if (dlg.getAddComments()) {
                        comment = "/* " + column_name + ( dlg.getFieldTypes() ? " (" + data_type + ") */ \n": " */\n");
                    } else {
                        comment = "\n";
                    }
                    column_default = QString::fromStdString(PQgetvalue(res, i, 1));

                    is_mandatory = QString::fromStdString(PQgetvalue(res, i, 3));
                    if (dlg.getOnlyMandatory() && is_mandatory == "NO")
                        continue;
                    if (is_mandatory != "NO" && column_default != "" && dlg.getRemoveMandatoryWithDefaults())
                        continue;
                    if (column_default != "")
                        insert_stmt << (i == 0 ? "    ":"   ,") + column_default + " " + comment;
                    else if (data_type == "character varying")
                        insert_stmt << (i == 0 ? "    '' ":"   ,'' ") + comment;
                    else if (data_type == "character")
                        insert_stmt << (i == 0 ? "    '' ":"   ,'' ") + comment;
                    else if (data_type == "date")
                        insert_stmt << (i == 0 ? "    '' ":"   ,'' ") + comment;
                    else if (data_type == "timestamp")
                        insert_stmt << (i == 0 ? "    '' ":"   ,'' ") + comment;
                    else if (data_type == "interval")
                        insert_stmt << (i == 0 ? "    '' ":"   ,'' ") + comment;
                    else if (data_type == "text")
                        insert_stmt << (i == 0 ? "    '' ":"   ,'' ") + comment;
                    else
                        insert_stmt << (i == 0 ? "       ":"   ,   ") + comment;
                }

                PQclear(res);
            }

            switch(dlg.getOnConflict()) {
            case DlgInsert::ON_CONFLICT_DO_NOTHING:
                insert_stmt << ") ON CONFLICT DO NOTHING;";
                break;
            case DlgInsert::ON_CONFLICT_UPDATE:
                insert_stmt << ") ON CONFLICT DO UPDATE SET \n<field>=<value>\n;";
                break;
            case DlgInsert::NO_CONFLICT_CLAUSE:
                insert_stmt << ");";
                break;
            }

        }

        return insert_stmt;
}

QStringList SQLGenerationPlugin::update_record(PGconn *connection, QString schema, QString table)
{
    QString column_name;
    QString comment;
    QString column_default;
    QString data_type;
    QString is_mandatory;
    int tuples;
    QStringList stmt;
    DlgUpdate dlg;

    const char *sql_list_fields =
        "SELECT column_name, column_default, data_type, is_nullable "
        "FROM information_schema.columns "
        "WHERE "
        "   table_schema = $1 "
        "   AND table_name = $2 ";

    if(dlg.exec()) {

        stmt << QString("UPDATE %1.%2 SET \n").arg(schema).arg(table);

        PGresult *res = createObjectList(connection, sql_list_fields, 2, schema.toStdString().c_str(),
                                         table.toStdString().c_str());

        if (PQresultStatus(res) != PGRES_TUPLES_OK)
        {
            stmt << PQerrorMessage(connection);
            PQclear(res);
        } else {

            tuples = PQntuples(res);

            for (int i = 0; i < tuples; i++) {
                column_name = QString::fromStdString(PQgetvalue(res, i, 0));
                data_type = QString::fromStdString(PQgetvalue(res, i, 2));
                if (dlg.getAddComments()) {
                    comment = "/* " + column_name + ( dlg.getFieldTypes() ? " (" + data_type + ") */ \n": " */\n");
                } else {
                    comment = "\n";
                }
                column_default = QString::fromStdString(PQgetvalue(res, i, 1));
                is_mandatory = QString::fromStdString(PQgetvalue(res, i, 3));

                if (data_type == "character varying")
                    stmt << (i == 0 ? "     ":"    ,") + column_name + " = ''" + comment;
                else if (data_type == "character")
                    stmt << (i == 0 ? "     ":"    ,") + column_name + " = ''" + comment;
                else if (data_type == "date")
                    stmt << (i == 0 ? "     ":"    ,") + column_name + " = ''" + comment;
                else if (data_type == "timestamp")
                    stmt << (i == 0 ? "     ":"    ,") + column_name + " = ''" + comment;
                else if (data_type == "interval")
                    stmt << (i == 0 ? "     ":"    ,") + column_name + " = ''" + comment;
                else if (data_type == "text")
                    stmt << (i == 0 ? "     ":"    ,") + column_name + " = ''" + comment;
                else
                    stmt << (i == 0 ? "     ":"    ,") + column_name + " = " + comment;
            }

            stmt << "WHERE <condition>; \n";

            PQclear(res);
        }

    }

    return stmt;
}

QStringList SQLGenerationPlugin::select_in_table(PGconn *connection, QString schema, QString table, int limit)
{
    QString column_name;
    int tuples;
    QStringList stmt;

    const char *sql_list_fields =
        "SELECT column_name, column_default, data_type, is_nullable "
        "FROM information_schema.columns "
        "WHERE "
        "   table_schema = $1 "
        "   AND table_name = $2 ";

    stmt << QString("SELECT \n");

    PGresult *res = createObjectList(connection, sql_list_fields, 2, schema.toStdString().c_str(),
                                     table.toStdString().c_str());

    if (PQresultStatus(res) != PGRES_TUPLES_OK)
    {
        stmt << PQerrorMessage(connection);
        PQclear(res);
    } else {

        tuples = PQntuples(res);

        for (int i = 0; i < tuples; i++) {
            column_name = QString::fromStdString(PQgetvalue(res, i, 0));
                stmt <<  (i == 0 ? "    ":"   ,") + column_name + "\n";
        }

        if (limit == -1)
            stmt << QString("FROM %1.%2; \n").arg(schema).arg(table);
        else
            stmt << QString("FROM %1.%2 \nLIMIT %3; \n").arg(schema).arg(table).arg(limit);

        PQclear(res);
    }

    return stmt;
}

#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2(pgdba_sqlgenplugin, SQLGenerationPlugin)
#endif // QT_VERSION < 0x050000
