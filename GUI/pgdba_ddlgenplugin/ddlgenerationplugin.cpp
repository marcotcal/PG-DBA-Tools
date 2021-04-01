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
    connect(tree, SIGNAL(itemActivated(QTreeWidgetItem*,int)), this, SLOT(processItem(QTreeWidgetItem*,int)));
    connect(tree, SIGNAL(itemClicked(QTreeWidgetItem*,int)), this, SLOT(processItem(QTreeWidgetItem*,int)));
}

void DDLGenerationPlugin::createTree(PGconn *value)
{
    QTreeWidgetItem *root = new QTreeWidgetItem();
    QTreeWidgetItem *schema_node;
    QTreeWidgetItem *schema;
    QStringList schemas_list;

    connection = value;

    if (value) {

        schemas_list = schemas(connection);
        root->setText(0, QString(PQdb(value)));
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
            schema->setData(0, Qt::UserRole, SCHEMA_ITEM);
            schema_node->addChild(schema);

        }

    }
}


bool DDLGenerationPlugin::run(PGconn *connection, int item, EditorItem *editor)
{

    return true;
}

void DDLGenerationPlugin::processItem(QTreeWidgetItem *item, int column)
{

    switch(item->data(0, Qt::UserRole).toInt()) {
    case SCHEMA_ITEM:
        if(item->childCount() == 0) processSchema(item);
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

void DDLGenerationPlugin::processSchema(QTreeWidgetItem *item) {


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

    table_list = tables(item->data(0, ROLE_SCHEMA_NAME).toString(), connection);

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

    view_list = views(item->data(0, ROLE_SCHEMA_NAME).toString(), connection);

    for(int j=0; j < view_list.count(); j++) {

        view = new QTreeWidgetItem();
        view->setText(0, view_list[j]);
        view->setIcon(0, QIcon(":/icons/images/icons/view.png"));
        view->setData(0, Qt::UserRole, VIEW_ITEM);
        item->addChild(view);

        triggers_node = new QTreeWidgetItem();
        triggers_node->setText(0, "Triggers");
        triggers_node->setIcon(0, QIcon(":/icons/images/icons/triggers.png"));
        view->addChild(triggers_node);

    }
}

void DDLGenerationPlugin::processSequences(QTreeWidgetItem *item)
{
    QStringList sequence_list;
    QTreeWidgetItem *sequence;

    sequence_list = sequences(item->data(0, ROLE_SCHEMA_NAME).toString(), connection);

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

    function_list = functions(item->data(0, ROLE_SCHEMA_NAME).toString(), connection);

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

    function_list = triggerFunctions(item->data(0, ROLE_SCHEMA_NAME).toString(), connection);

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

    // primary key
    constr = constraints(schema, table, "p", connection);
    if (constr.count() > 0) {
        constraint = new QTreeWidgetItem();
        constraint->setText(0, constr[0]);
        constraint->setIcon(0, QIcon(":/icons/images/icons/primary_key.png"));
        constraint->setData(0, ROLE_ITEM_TYPE, FUNCTION_ITEM);
        constraint->setData(0, ROLE_TABLE_NAME, table);
        constraint->setData(0, ROLE_SCHEMA_NAME, schema);
        constraint->setData(0, ROLE_CONTRAINT_TYPE, "P");
        item->addChild(constraint);
    }

    // unique key
    constr = constraints(schema, table, "u", connection);
    for(int j=0; j < constr.count(); j++) {
        constraint = new QTreeWidgetItem();
        constraint->setText(0, constr[j]);
        constraint->setIcon(0, QIcon(":/icons/images/icons/unique_key.png"));
        constraint->setData(0, ROLE_ITEM_TYPE, FUNCTION_ITEM);
        constraint->setData(0, ROLE_TABLE_NAME, table);
        constraint->setData(0, ROLE_SCHEMA_NAME, schema);
        constraint->setData(0, ROLE_CONTRAINT_TYPE, "U");
        item->addChild(constraint);
    }

    // foreign key
    constr = constraints(schema, table, "f", connection);
    for(int j=0; j < constr.count(); j++) {
        constraint = new QTreeWidgetItem();
        constraint->setText(0, constr[j]);
        constraint->setIcon(0, QIcon(":/icons/images/icons/foreign_key.png"));
        constraint->setData(0, ROLE_ITEM_TYPE, FUNCTION_ITEM);
        constraint->setData(0, ROLE_TABLE_NAME, table);
        constraint->setData(0, ROLE_SCHEMA_NAME, schema);
        constraint->setData(0, ROLE_CONTRAINT_TYPE, "F");
        item->addChild(constraint);
    }

    // check constraint
    constr = constraints(schema, table, "c", connection);
    for(int j=0; j < constr.count(); j++) {
        constraint = new QTreeWidgetItem();
        constraint->setText(0, constr[j]);
        constraint->setIcon(0, QIcon(":/icons/images/icons/check_constraint.png"));
        constraint->setData(0, ROLE_ITEM_TYPE, FUNCTION_ITEM);
        constraint->setData(0, ROLE_TABLE_NAME, table);
        constraint->setData(0, ROLE_SCHEMA_NAME, schema);
        constraint->setData(0, ROLE_CONTRAINT_TYPE, "C");
        item->addChild(constraint);
    }
}

void DDLGenerationPlugin::processTriggers(QTreeWidgetItem *item)
{
    QString schema = item->data(0, ROLE_SCHEMA_NAME).toString();
    QString table = item->data(0, ROLE_TABLE_NAME).toString();
    QStringList trigger_list;
    QTreeWidgetItem *trigger;

    trigger_list = triggers(schema, table, connection);

    for(int j=0; j < trigger_list.count(); j++) {
        trigger = new QTreeWidgetItem();
        trigger->setText(0, trigger_list[j]);
        trigger->setIcon(0, QIcon(":/icons/images/icons/trigger.png"));
        trigger->setData(0, ROLE_ITEM_TYPE, TRIGGER_ITEM);
        trigger->setData(0, ROLE_TABLE_NAME, table);
        trigger->setData(0, ROLE_SCHEMA_NAME, schema);
        trigger->setData(0, ROLE_CONTRAINT_TYPE, "C");
        item->addChild(trigger);
    }

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

QStringList DDLGenerationPlugin::sequences(QString schema, PGconn *connection)
{
    const char *sql =
        "SELECT "
        "  sequence_schema, "
        "  sequence_name "
        "FROM information_schema.sequences "
        "WHERE sequence_schema = $1 ";
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

QStringList DDLGenerationPlugin::functions(QString schema, PGconn *connection)
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

QStringList DDLGenerationPlugin::triggerFunctions(QString schema, PGconn *connection)
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

QStringList DDLGenerationPlugin::constraints(QString schema, QString table, char *ctype, PGconn *connection)
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
    const char *params[3];
    QStringList list;
    int tuples;

    params[0] = schema.toStdString().c_str();
    params[1] = table.toStdString().c_str();
    params[2] = ctype;

    if (PQstatus(connection) == CONNECTION_OK) {

        PGresult *res =  PQexecParams(connection, sql, 3, NULL, params, NULL, NULL, 0);

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

QStringList DDLGenerationPlugin::triggers(QString schema, QString table, PGconn *connection)
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

    const char *params[2];
    QStringList list;
    int tuples;

    params[0] = schema.toStdString().c_str();
    params[1] = table.toStdString().c_str();

    if (PQstatus(connection) == CONNECTION_OK) {

        PGresult *res =  PQexecParams(connection, sql, 2, NULL, params, NULL, NULL, 0);

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
