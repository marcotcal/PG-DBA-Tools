#include "ddlgenerationplugin.h"
#include <QDebug>
#include <plugintabwidget.h>

DDLGenerationPlugin::DDLGenerationPlugin(QObject *parent) :
    QObject(parent)
{

}

void DDLGenerationPlugin::createTree(PGconn *connection, QTreeWidget *tree)
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

QStringList DDLGenerationPlugin::run(PGconn *connection, int item)
{

    QStringList resp;

    switch(item) {
    case DDL_TEST:
        resp.append("-- Plugin Test.\n");
        resp.append("SELECT 'TESTING DDL GENERATION PLUGIN'\n");
        resp.append("-- End.\n");
        break;
    case DDL_CREATE_ALL_SCHEMAS:
        resp = createAllSchemas(connection);
        break;
    default:
        return QStringList();
    }

    return resp;
}

void DDLGenerationPlugin::updateFunctionList(QTreeWidgetItem *item, QListWidget *list)
{        
    QListWidgetItem *list_item;
    int item_type;

    item_type = item->data(0, ROLE_ITEM_TYPE).toInt();

    list->clear();

    switch(item_type) {
    case SCHEMAS_ITEM:
        list_item = new QListWidgetItem("Create all schemas");
        list->addItem(list_item);
        list_item->setData(ROLE_ITEM_TYPE, DDL_CREATE_ALL_SCHEMAS);
        list_item = new QListWidgetItem("Drop all schemas");
        list->addItem(list_item);
        list_item->setData(ROLE_ITEM_TYPE, DDL_DROP_ALL_SCHEMAS);
        break;
    case SCHEMA_ITEM:
        list_item = new QListWidgetItem("Create schema");
        list->addItem(list_item);
        list_item->setData(ROLE_ITEM_TYPE, DDL_CREATE_SCHEMA);
        list_item = new QListWidgetItem("Drop schema");
        list->addItem(list_item);
        list_item->setData(ROLE_ITEM_TYPE, DDL_DROP_SCHEMA);
        break;
    case SEQUENCES_ITEM:
        list_item = new QListWidgetItem("Resset Sequences");
        list->addItem(list_item);
        list_item->setData(ROLE_ITEM_TYPE, DDL_RESET_SEQUENCES);
        list_item = new QListWidgetItem("Update Sequences");
        list->addItem(list_item);
        list_item->setData(ROLE_ITEM_TYPE, DDL_UPDATE_SEQUENCES);
        break;
    case SEQUENCE_ITEM:
        list_item = new QListWidgetItem("Resset Sequence");
        list->addItem(list_item);
        list_item->setData(ROLE_ITEM_TYPE, DDL_RESET_SEQUENCE);
        list_item = new QListWidgetItem("Update Sequence");
        list->addItem(list_item);
        list_item->setData(ROLE_ITEM_TYPE, DDL_UPDATE_SEQUENCE);
        break;
    case FUNCTIONS_ITEM:
        list_item = new QListWidgetItem("Create New Function");
        list->addItem(list_item);

        list_item = new QListWidgetItem("Drop all Functions");
        list->addItem(list_item);

        list_item = new QListWidgetItem("Create all Functions");
        list->addItem(list_item);
        break;
    case FUNCTION_ITEM:
        list_item = new QListWidgetItem("Drop Function");
        list->addItem(list_item);

        list_item = new QListWidgetItem("Create or Replace Function");
        list->addItem(list_item);

        list_item = new QListWidgetItem("Script Alter Function Parameters");
        list->addItem(list_item);
        break;
    case TRIGGER_FUNCTIONS_ITEM:
        list_item = new QListWidgetItem("Create New Function");
        list->addItem(list_item);

        list_item = new QListWidgetItem("Drop all Functions and related triggers");
        list->addItem(list_item);

        list_item = new QListWidgetItem("Create all Functions");
        list->addItem(list_item);
        break;
    case TRIGGER_FUNCTION_ITEM:
        list_item = new QListWidgetItem("Drop Function and related triggers");
        list->addItem(list_item);

        list_item = new QListWidgetItem("Create or Replace Function");
        list->addItem(list_item);

        list_item = new QListWidgetItem("Script Alter Function Parameters");
        list->addItem(list_item);
        break;
    }
}

void DDLGenerationPlugin::processItem(QTreeWidgetItem *item, int column)
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
    case SEQUENCES_ITEM:
        if(item->childCount() == 0) processSequences(item);
        break;
    case FUNCTIONS_ITEM:
        if(item->childCount() == 0) processFunctions(item);
        break;
    case TRIGGER_FUNCTIONS_ITEM:
        if(item->childCount() == 0) processTrigerFunctions(item);
        break;
    case CONSTRAINTS_ITEM:
        if(item->childCount() == 0) processConstraints(item);
        break;
    case TRIGGERS_ITEM:
        if(item->childCount() == 0) processTriggers(item);
        break;
    }
}


QStringList DDLGenerationPlugin::createObjectList(PGconn *connection, const char *sql, int return_col, int param_count, ...)
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

void DDLGenerationPlugin::processSchemas(QTreeWidgetItem *item) {


    QTreeWidgetItem *table_node;
    QTreeWidgetItem *view_node;
    QTreeWidgetItem *function_node;
    QTreeWidgetItem *sequence_node;
    QTreeWidgetItem *trigger_function_node;

    if (item->childCount() > 0)
        return;

    sequence_node = new QTreeWidgetItem();
    sequence_node->setText(0, "Sequences");
    sequence_node->setIcon(0, QIcon(":/icons/images/icons/sequences.png"));
    sequence_node->setData(0, ROLE_ITEM_TYPE, SEQUENCES_ITEM);
    sequence_node->setData(0, ROLE_SCHEMA_NAME, item->text(0));
    item->addChild(sequence_node);

    function_node = new QTreeWidgetItem();
    function_node->setText(0, "Functions");
    function_node->setIcon(0, QIcon(":/icons/images/icons/functions.png"));
    function_node->setData(0, ROLE_ITEM_TYPE, FUNCTIONS_ITEM);
    function_node->setData(0, ROLE_SCHEMA_NAME, item->text(0));
    item->addChild(function_node);

    trigger_function_node = new QTreeWidgetItem();
    trigger_function_node->setText(0, "Trigger Functions");
    trigger_function_node->setIcon(0, QIcon(":/icons/images/icons/trigger_functions.png"));
    trigger_function_node->setData(0, Qt::UserRole, TRIGGER_FUNCTIONS_ITEM);
    trigger_function_node->setData(0, ROLE_SCHEMA_NAME, item->text(0));
    item->addChild(trigger_function_node);

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

void DDLGenerationPlugin::processTables(QTreeWidgetItem *item)
{

    //QTreeWidgetItem *index_node;
    QTreeWidgetItem *constraints_node;
    QTreeWidgetItem *triggers_node;
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

        constraints_node = new QTreeWidgetItem();
        constraints_node->setText(0, "Constraints");
        constraints_node->setIcon(0, QIcon(":/icons/images/icons/constraints.png"));
        constraints_node->setData(0, ROLE_ITEM_TYPE, CONSTRAINTS_ITEM);
        constraints_node->setData(0, ROLE_SCHEMA_NAME, item->data(0, ROLE_SCHEMA_NAME).toString());
        constraints_node->setData(0, ROLE_TABLE_NAME, table_list[j]);
        table->addChild(constraints_node);

        triggers_node = new QTreeWidgetItem();
        triggers_node->setText(0, "Triggers");
        triggers_node->setIcon(0, QIcon(":/icons/images/icons/triggers.png"));
        triggers_node->setData(0, ROLE_ITEM_TYPE, TRIGGERS_ITEM);
        triggers_node->setData(0, ROLE_SCHEMA_NAME, item->data(0, ROLE_SCHEMA_NAME).toString());
        triggers_node->setData(0, ROLE_TABLE_NAME, table_list[j]);
        table->addChild(triggers_node);

    }
}

void DDLGenerationPlugin::processViews(QTreeWidgetItem *item)
{
    QTreeWidgetItem *triggers_node;
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

        triggers_node = new QTreeWidgetItem();
        triggers_node->setText(0, "Triggers");
        triggers_node->setIcon(0, QIcon(":/icons/images/icons/triggers.png"));
        triggers_node->setData(0, ROLE_ITEM_TYPE, TRIGGERS_ITEM);
        triggers_node->setData(0, ROLE_SCHEMA_NAME, item->data(0, ROLE_SCHEMA_NAME).toString());
        triggers_node->setData(0, ROLE_TABLE_NAME, view_list[j]);
        view->addChild(triggers_node);

    }
}

void DDLGenerationPlugin::processSequences(QTreeWidgetItem *item)
{
    QStringList sequence_list;
    QTreeWidgetItem *sequence;

    PGconn *connection = trees[item->treeWidget()];

    sequence_list = sequences(connection, item->data(0, ROLE_SCHEMA_NAME).toString());

    for(int j=0; j < sequence_list.count(); j++) {

        sequence = new QTreeWidgetItem();
        sequence->setText(0, sequence_list[j]);
        sequence->setIcon(0, QIcon(":/icons/images/icons/sequence.png"));
        sequence->setData(0, ROLE_ITEM_TYPE, SEQUENCE_ITEM);
        item->addChild(sequence);

    }
}

void DDLGenerationPlugin::processFunctions(QTreeWidgetItem *item)
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

void DDLGenerationPlugin::processTrigerFunctions(QTreeWidgetItem *item)
{
    QStringList function_list;
    QTreeWidgetItem *function;

    PGconn *connection = trees[item->treeWidget()];

    function_list = triggerFunctions(connection, item->data(0, ROLE_SCHEMA_NAME).toString());

    for(int j=0; j < function_list.count(); j++) {

        function = new QTreeWidgetItem();
        function->setText(0, function_list[j]);
        function->setIcon(0, QIcon(":/icons/images/icons/trigger_function.png"));
        function->setData(0, ROLE_ITEM_TYPE, TRIGGER_FUNCTION_ITEM);
        item->addChild(function);

    }
}

void DDLGenerationPlugin::processConstraints(QTreeWidgetItem *item)
{
    QString schema = item->data(0, ROLE_SCHEMA_NAME).toString();
    QString table = item->data(0, ROLE_TABLE_NAME).toString();
    QStringList constr;
    QTreeWidgetItem *constraint;

    PGconn *connection = trees[item->treeWidget()];

    // primary key
    constr = constraints(connection, schema, table, "p");
    if (constr.count() > 0) {
        constraint = new QTreeWidgetItem();
        constraint->setText(0, constr[0]);
        constraint->setIcon(0, QIcon(":/icons/images/icons/primary_key.png"));
        constraint->setData(0, ROLE_ITEM_TYPE, FUNCTION_ITEM);
        constraint->setData(0, ROLE_TABLE_NAME, table);
        constraint->setData(0, ROLE_SCHEMA_NAME, schema);
        constraint->setData(0, ROLE_CONSTRAINT_TYPE, "P");
        item->addChild(constraint);
    }

    // unique key
    constr = constraints(connection, schema, table, "u");
    for(int j=0; j < constr.count(); j++) {
        constraint = new QTreeWidgetItem();
        constraint->setText(0, constr[j]);
        constraint->setIcon(0, QIcon(":/icons/images/icons/unique_key.png"));
        constraint->setData(0, ROLE_ITEM_TYPE, FUNCTION_ITEM);
        constraint->setData(0, ROLE_TABLE_NAME, table);
        constraint->setData(0, ROLE_SCHEMA_NAME, schema);
        constraint->setData(0, ROLE_CONSTRAINT_TYPE, "U");
        item->addChild(constraint);
    }

    // foreign key
    constr = constraints(connection, schema, table, "f");
    for(int j=0; j < constr.count(); j++) {
        constraint = new QTreeWidgetItem();
        constraint->setText(0, constr[j]);
        constraint->setIcon(0, QIcon(":/icons/images/icons/foreign_key.png"));
        constraint->setData(0, ROLE_ITEM_TYPE, FUNCTION_ITEM);
        constraint->setData(0, ROLE_TABLE_NAME, table);
        constraint->setData(0, ROLE_SCHEMA_NAME, schema);
        constraint->setData(0, ROLE_CONSTRAINT_TYPE, "F");
        item->addChild(constraint);
    }

    // check constraint
    constr = constraints(connection, schema, table, "c");
    for(int j=0; j < constr.count(); j++) {
        constraint = new QTreeWidgetItem();
        constraint->setText(0, constr[j]);
        constraint->setIcon(0, QIcon(":/icons/images/icons/check_constraint.png"));
        constraint->setData(0, ROLE_ITEM_TYPE, FUNCTION_ITEM);
        constraint->setData(0, ROLE_TABLE_NAME, table);
        constraint->setData(0, ROLE_SCHEMA_NAME, schema);
        constraint->setData(0, ROLE_CONSTRAINT_TYPE, "C");
        item->addChild(constraint);
    }
}

void DDLGenerationPlugin::processTriggers(QTreeWidgetItem *item)
{
    QString schema = item->data(0, ROLE_SCHEMA_NAME).toString();
    QString table = item->data(0, ROLE_TABLE_NAME).toString();
    QStringList trigger_list;
    QTreeWidgetItem *trigger;

    PGconn *connection = trees[item->treeWidget()];

    trigger_list = triggers(connection, schema, table);

    for(int j=0; j < trigger_list.count(); j++) {
        trigger = new QTreeWidgetItem();
        trigger->setText(0, trigger_list[j]);
        trigger->setIcon(0, QIcon(":/icons/images/icons/trigger.png"));
        trigger->setData(0, ROLE_ITEM_TYPE, TRIGGER_ITEM);
        trigger->setData(0, ROLE_TABLE_NAME, table);
        trigger->setData(0, ROLE_SCHEMA_NAME, schema);
        trigger->setData(0, ROLE_CONSTRAINT_TYPE, "C");
        item->addChild(trigger);
    }

}

QStringList DDLGenerationPlugin::createAllSchemas(PGconn *connection)
{
    const char *sql =
        "SELECT "
        "    'CREATE SCHEMA IF NOT EXISTS ' || schema_name || ' AUTHORIZATION ' || schema_owner || E';\n' AS create_schema "
        "FROM information_schema.schemata "
        "WHERE schema_name NOT IN ('public', 'information_schema') AND schema_name !~ '^pg_' ";
    return createObjectList(connection, sql, 0, 0);
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
    return createObjectList(connection, sql, 0, 0);
}

QStringList DDLGenerationPlugin::schemas(PGconn *connection)
{
    const char *sql =
        "SELECT schema_name "
        "FROM information_schema.schemata "
        "WHERE schema_name NOT IN ('information_schema') AND schema_name !~ '^pg_' ";
    return createObjectList(connection, sql, 0, 0);
}

QStringList DDLGenerationPlugin::tables(PGconn *connection, QString schema)
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

QStringList DDLGenerationPlugin::views(PGconn *connection, QString schema)
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

QStringList DDLGenerationPlugin::sequences(PGconn *connection, QString schema)
{
    const char *sql =
        "SELECT "
        "  sequence_schema, "
        "  sequence_name "
        "FROM information_schema.sequences "
        "WHERE sequence_schema = $1 ";
    return createObjectList(connection, sql, 1, 1, schema.toStdString().c_str());
}

QStringList DDLGenerationPlugin::functions(PGconn *connection, QString schema)
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

QStringList DDLGenerationPlugin::triggerFunctions(PGconn *connection, QString schema)
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
            "    t.typname = 'trigger' AND "
            "    n.nspname = $1 ";
    return createObjectList(connection, sql, 1, 1, schema.toStdString().c_str());
}

QStringList DDLGenerationPlugin::constraints(PGconn *connection, QString schema, QString table, char *ctype)
{
    /*
     * u = unique
     * p = primary key
     * f = foreign key
     * c = check
     */

    const char *sql =
            "SELECT con.conname, con.contype "
            "FROM "
            "    pg_catalog.pg_constraint con "
            "    INNER JOIN pg_catalog.pg_class rel ON rel.oid = con.conrelid "
            "    INNER JOIN pg_catalog.pg_namespace nsp ON nsp.oid = connamespace "
            "WHERE "
            "    nsp.nspname = $1 "
            "    AND rel.relname = $2 "
            "    AND con.contype = $3 ";
    return createObjectList(connection,
                            sql, 0, 3,
                            schema.toStdString().c_str(),
                            table.toStdString().c_str(),
                            ctype);
}

QStringList DDLGenerationPlugin::triggers(PGconn *connection, QString schema, QString table)
{
    /* for detailed use
    const char *sql =
            "SELECT event_object_schema, "
            "       event_object_table, "
            "       trigger_schema, "
            "       trigger_name, "
            "       string_agg(event_manipulation, ',') as event, "
            "       action_timing, "
            "       action_condition, "
            "       action_statement "
            "FROM information_schema.triggers "
            "WHERE "
            "    event_object_schema = $1 "
            "    AND event_object_table = $2 ";
            "GROUP BY 1,2,3,4,6,7,8 ";
    */
    const char *sql =
            "SELECT "
            "    trg.tgname "
            "FROM "
            "    pg_trigger trg "
            "    INNER JOIN pg_class tbl ON trg.tgrelid = tbl.oid "
            "    INNER JOIN pg_namespace n ON tbl.relnamespace = n.oid "
            "WHERE "
            "    tbl.relname !~ '^pg_' "
            "    AND tgisinternal = FALSE "
            "    AND n.nspname = $1 "
            "    AND tbl.relname = $2 ";
    return createObjectList(connection,
                            sql, 0, 2,
                            schema.toStdString().c_str(),
                            table.toStdString().c_str());

}

#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2(pgdba_ddlgenplugin, DDLGenerationPlugin)
#endif // QT_VERSION < 0x050000
