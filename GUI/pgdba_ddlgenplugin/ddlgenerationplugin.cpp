#include "ddlgenerationplugin.h"
#include <QDebug>
#include <plugintabwidget.h>

/**
   pg_proc change history from https://pgpedia.info/p/pg_proc.html

    Change history
    Note: may not show all changes prior to PostgreSQL 8.2.

    PostgreSQL 14
    column prosqlbody added (commit e717a9a1)
    PostgreSQL 12
    column protransform renamed to prosupport (commit 74dfe58a)
    PostgreSQL 11
    columns proisagg and proiswindow replaced by prokind (commit fd1a421f)
    PostgreSQL 9.6
    column proparallel added (commit 7aea8e4f)
    PostgreSQL 9.5
    column protrftypes added (commit cac76582)
    PostgreSQL 9.2
    column protransform added (commit 8f9fe6ed)
    column proleakproof added (commit cd30728f)
    PostgreSQL 9.1
    column proargdefaults changed to type pg_node_tree (commit 303696c3)
    PostgreSQL 8.4
    column procost added (commit 5a7471c3)
    column prorows added (commit 5a7471c3)
    column provariadic added (commit 6563e9e2)
    PostgreSQL 8.3
    column procost added (commit 5a7471c3)
    column prorows added (commit 5a7471c3)
    column proconfig added (commit 2abae34a)
    PostgreSQL 7.3
    following columns removed (commit 739adf32):
    proimplicit
    probyte_pct
    properbyte_cpu
    propercall_cpu
    prooutin_ratio

**/

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
            schema->setData(0, ROLE_SCHEMA_NAME, schemas_list[i]);
            schema_node->addChild(schema);

        }

    }
}

QStringList DDLGenerationPlugin::run(QTreeWidgetItem *tree_item, PGconn *connection, int command)
{

    QStringList resp;
    QString schema_name = tree_item->data(0, ROLE_SCHEMA_NAME).toString();
    QString sequence_name = tree_item->data(0, ROLE_SEQUENCE_NAME).toString();
    QString function_name = tree_item->data(0, ROLE_FUNCTION_NAME).toString();
    QString procedure_name = tree_item->data(0, ROLE_PROCEDURE_NAME).toString();
    QString table_name = tree_item->data(0, ROLE_TABLE_NAME).toString();
    QString constraint_name = tree_item->data(0, ROLE_CONSTRAINT_NAME).toString();
    QString trigger_name = tree_item->data(0, ROLE_TRIGGER_NAME).toString();
    QString type_name = tree_item->data(0, ROLE_TYPE_NAME).toString();
    QString view_name = tree_item->data(0, ROLE_VIEW_NAME).toString();

    switch(command) {
    case DDL_TEST:
        resp.append("-- Plugin Test.\n");
        resp.append("SELECT 'TESTING DDL GENERATION PLUGIN'\n");
        resp.append("-- End.\n");
        break;
    case DDL_VERSION:
        resp << "-- " + getDatabaseVersionString(connection) + "\n";
        break;
    case DDL_CREATE_ALL_SCHEMAS:
        resp = createAllSchemas(connection);
        break;
    case DDL_DROP_ALL_SCHEMAS:
        resp = dropAllSchemas(connection);
        break;
    case DDL_CREATE_SCHEMA:
        resp = createSchema(connection, schema_name);
        break;
    case DDL_DROP_SCHEMA:
        resp = dropSchema(connection, schema_name);
        break;
    case DDL_RESET_ALL_SEQUENCES:
        resp = resetAllSequences(connection);
        break;
    case DDL_UPDATE_ALL_SEQUENCES:
        resp = updatetAllSequences(connection);
        break;
    case DDL_RESET_SEQUENCES:
        resp = resetSequences(connection, schema_name);
        break;
    case DDL_UPDATE_SEQUENCES:
        resp = updateSequences(connection, schema_name);
        break;
    case DDL_RESET_SEQUENCE:
        resp = resetSequence(connection, schema_name, sequence_name);
        break;
    case DDL_UPDATE_SEQUENCE:
        resp = updateSequence(connection, schema_name, sequence_name);
        break;
    case DDL_CREATE_ALL_TRIGGERS:
        resp = createAllTriggers(connection);
        break;
    case DDL_DROP_ALL_TRIGGERS:
        resp = dropAllTriggers(connection);
        break;
    case DDL_ENABLE_ALL_TRIGGERS:
        resp = enableAllTriggers(connection);
        break;
    case DDL_DISABLE_ALL_TRIGGERS:
        resp = disableAllTriggers(connection);
        break;
    case DDL_CREATE_TRIGGERS:
        resp = createTriggers(connection, schema_name, table_name);
        break;
    case DDL_DROP_TRIGGERS:
        resp = dropTriggers(connection, schema_name, table_name);
        break;
    case DDL_ENABLE_TRIGGERS:
        resp = enableTriggers(connection, schema_name, table_name);
        break;
    case DDL_DISABLE_TRIGGERS:
        resp = disableTriggers(connection, schema_name, table_name);
        break;
    case DDL_CREATE_TRIGGER:
        resp = createTrigger(connection, schema_name, table_name, trigger_name);
        break;
    case DDL_DROP_TRIGGER:
        resp = dropTrigger(connection, schema_name, table_name, trigger_name);
        break;
    case DDL_ENABLE_TRIGGER:
        resp = enableTrigger(connection, schema_name, table_name, trigger_name);
        break;
    case DDL_DISABLE_TRIGGER:
        resp = disableTrigger(connection, schema_name, table_name, trigger_name);
        break;
    case DDL_CREATE_FUNCTIONS:
        resp = createFunctions(connection, schema_name, false);
        break;
    case DDL_CREATE_FUNCTION:
        resp = createFunction(connection, schema_name, function_name);
        break;
    case DDL_DROP_FUNCTIONS:
        resp = dropFunctions(connection, schema_name);
        break;
    case DDL_DROP_FUNCTION:
        resp = dropFunction(connection, schema_name, function_name);
        break;    
    case DDL_FUNCTION_OWNER:
         resp = functionOwner(connection, schema_name, function_name);
         break;
    case DDL_CREATE_PROCEDURES:

        break;
    case DDL_CREATE_PROCEDURE:
        resp = createProcedure(connection, schema_name, procedure_name);
        break;
    case DDL_DROP_PROCEDURE:
        resp = dropProcedure(connection, schema_name, procedure_name);
        break;
    case DDL_PROCEDURE_OWNER:
         resp = procedureOwner(connection, schema_name, procedure_name);
         break;
    case DDL_CREATE_TRIGGER_FUNCTIONS:
        resp = createFunctions(connection, schema_name, true);
        break;
    case DDL_CREATE_TABLE:
        resp = createTable(connection, schema_name, table_name);
        break;
    case DDL_CREATE_PRIMARY_KEY:
        resp = createPrimaryKey(connection, schema_name, table_name, constraint_name);
        break;
    case DDL_DROP_PRIMARY_KEY:
        resp = dropPrimaryKey(connection, schema_name, table_name, constraint_name);
        break;
    case DDL_CREATE_UNIQUE_KEY:
        resp = createUniqueKey(connection, schema_name, table_name, constraint_name);
        break;
    case DDL_DROP_UNIQUE_KEY:
        resp = dropUniqueKey(connection, schema_name, table_name, constraint_name);
        break;
    case DDL_CREATE_FOREIGN_KEY:
        resp = createForeignKey(connection, schema_name, table_name, constraint_name);
        break;
    case DDL_DROP_FOREIGN_KEY:
        resp = dropForeignKey(connection, schema_name, table_name, constraint_name);
        break;
    case DDL_CREATE_CONSTRAINTS:
        resp = createConstraints(connection, schema_name, table_name);
        break;
    case DDL_DROP_CONSTRAINTS:
        resp = dropConstraints(connection, schema_name, table_name);
        break;
    case DDL_CREATE_TYPES:
        break;
    case DDL_CREATE_TYPE:
        resp = createType(connection, schema_name, type_name);
        break;
    case DDL_DROP_TYPES:
        resp = dropTypes(connection, schema_name);
        break;
    case DDL_DROP_TYPE:
        resp = dropType(connection, schema_name, type_name);
        break;
    case DDL_DESCRIBE_TABLE_FIELDS:
        resp = describeTableFields(connection, schema_name, table_name);
        break;
    case DDL_CREATE_VIEW:
        resp = createView(connection, schema_name, view_name);
        break;
    case DDL_DROP_VIEW:
        resp = dropView(connection, schema_name, view_name);
        break;
    case DDL_VIEW_OWNER:
        resp = viewOwner(connection, schema_name, view_name);
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
    QString const_type;

    item_type = item->data(0, ROLE_ITEM_TYPE).toInt();
    if (item_type == CONSTRAINT_ITEM) {
        const_type = item->data(0, ROLE_CONSTRAINT_TYPE).toString();
    }

    list->clear();

    switch(item_type) {
    case DATABASE_ITEM:

        list_item = new QListWidgetItem("PostgreSQL Server Version");
        list->addItem(list_item);
        list_item->setData(ROLE_ITEM_TYPE, DDL_VERSION);

        break;

    case SCHEMAS_ITEM:

        list_item = new QListWidgetItem("Create all schemas");
        list->addItem(list_item);
        list_item->setData(ROLE_ITEM_TYPE, DDL_CREATE_ALL_SCHEMAS);

        list_item = new QListWidgetItem("Drop all schemas");
        list->addItem(list_item);
        list_item->setData(ROLE_ITEM_TYPE, DDL_DROP_ALL_SCHEMAS);

        list_item = new QListWidgetItem("Reset all sequences in all schemas");
        list->addItem(list_item);
        list_item->setData(ROLE_ITEM_TYPE, DDL_RESET_ALL_SEQUENCES);

        list_item = new QListWidgetItem("Update all sequences in all schemas");
        list->addItem(list_item);
        list_item->setData(ROLE_ITEM_TYPE, DDL_UPDATE_ALL_SEQUENCES);

        list_item = new QListWidgetItem("Create all triggers in all schemas");
        list->addItem(list_item);
        list_item->setData(ROLE_ITEM_TYPE, DDL_CREATE_ALL_TRIGGERS);

        list_item = new QListWidgetItem("Drop all triggers in all schemas");
        list->addItem(list_item);
        list_item->setData(ROLE_ITEM_TYPE, DDL_DROP_ALL_TRIGGERS);

        list_item = new QListWidgetItem("Disable all triggers in all schemas");
        list->addItem(list_item);
        list_item->setData(ROLE_ITEM_TYPE, DDL_DISABLE_ALL_TRIGGERS);

        list_item = new QListWidgetItem("Enable all triggers in all schemas");
        list->addItem(list_item);
        list_item->setData(ROLE_ITEM_TYPE, DDL_ENABLE_ALL_TRIGGERS);

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

        list_item = new QListWidgetItem("Create Functions");
        list->addItem(list_item);
        list_item->setData(ROLE_ITEM_TYPE, DDL_CREATE_FUNCTIONS);

        list_item = new QListWidgetItem("Drop Functions");
        list->addItem(list_item);
        list_item->setData(ROLE_ITEM_TYPE, DDL_DROP_FUNCTIONS);

        break;    
    case FUNCTION_ITEM:
        list_item = new QListWidgetItem("Drop Function");
        list->addItem(list_item);
        list_item->setData(ROLE_ITEM_TYPE, DDL_DROP_FUNCTION);

        list_item = new QListWidgetItem("Create or Replace Function");
        list->addItem(list_item);
        list_item->setData(ROLE_ITEM_TYPE, DDL_CREATE_FUNCTION);

        list_item = new QListWidgetItem("Function Owner");
        list->addItem(list_item);
        list_item->setData(ROLE_ITEM_TYPE, DDL_FUNCTION_OWNER);
        break;
    case PROCEDURES_ITEM:
        list_item = new QListWidgetItem("Create New Procedure");
        list->addItem(list_item);

        list_item = new QListWidgetItem("Create Procedures");
        list->addItem(list_item);
        list_item->setData(ROLE_ITEM_TYPE, DDL_CREATE_FUNCTIONS);

        list_item = new QListWidgetItem("Drop Procedures");
        list->addItem(list_item);
        list_item->setData(ROLE_ITEM_TYPE, DDL_DROP_FUNCTIONS);

        break;
    case PROCEDURE_ITEM:
        list_item = new QListWidgetItem("Drop Procedure");
        list->addItem(list_item);
        list_item->setData(ROLE_ITEM_TYPE, DDL_DROP_PROCEDURE);

        list_item = new QListWidgetItem("Create or Replace Procedure");
        list->addItem(list_item);
        list_item->setData(ROLE_ITEM_TYPE, DDL_CREATE_PROCEDURE);

        list_item = new QListWidgetItem("Procedure Owner");
        list_item->setData(ROLE_ITEM_TYPE, DDL_PROCEDURE_OWNER);
        list->addItem(list_item);
        break;
    case TRIGGER_FUNCTIONS_ITEM:
        list_item = new QListWidgetItem("Create New Function");
        list->addItem(list_item);

        list_item = new QListWidgetItem("Drop all Functions and related triggers");
        list->addItem(list_item);

        list_item = new QListWidgetItem("Create all Functions");
        list->addItem(list_item);
        list_item->setData(ROLE_ITEM_TYPE, DDL_CREATE_TRIGGER_FUNCTIONS);
        break;
    case TRIGGER_FUNCTION_ITEM:
        list_item = new QListWidgetItem("Drop Function and related triggers");
        list->addItem(list_item);

        list_item = new QListWidgetItem("Create or Replace Function");
        list_item->setData(ROLE_ITEM_TYPE, DDL_CREATE_FUNCTION);
        list->addItem(list_item);

        list_item = new QListWidgetItem("Script Alter Function Parameters");
        list->addItem(list_item);
        break;
    case TABLE_ITEM:
        list_item = new QListWidgetItem("Create Table");
        list->addItem(list_item);
        list_item->setData(ROLE_ITEM_TYPE, DDL_CREATE_TABLE);

        list_item = new QListWidgetItem("Describe Table Fields");
        list->addItem(list_item);
        list_item->setData(ROLE_ITEM_TYPE, DDL_DESCRIBE_TABLE_FIELDS);
        break;
    case CONSTRAINTS_ITEM:
        list_item = new QListWidgetItem("Create Constraints");
        list->addItem(list_item);
        list_item->setData(ROLE_ITEM_TYPE, DDL_CREATE_CONSTRAINTS);

        list_item = new QListWidgetItem("Drop Constraints");
        list->addItem(list_item);
        list_item->setData(ROLE_ITEM_TYPE, DDL_DROP_CONSTRAINTS);
        break;
    case CONSTRAINT_ITEM:
        if (const_type == "P") {
            list_item = new QListWidgetItem("Create Primary Key");
            list->addItem(list_item);
            list_item->setData(ROLE_ITEM_TYPE, DDL_CREATE_PRIMARY_KEY);

            list_item = new QListWidgetItem("Drop Primary Key");
            list->addItem(list_item);
            list_item->setData(ROLE_ITEM_TYPE, DDL_DROP_PRIMARY_KEY);
        } else if (const_type == "U") {
            list_item = new QListWidgetItem("Create Unique Key");
            list->addItem(list_item);
            list_item->setData(ROLE_ITEM_TYPE, DDL_CREATE_UNIQUE_KEY);

            list_item = new QListWidgetItem("Drop Unique Key");
            list->addItem(list_item);
            list_item->setData(ROLE_ITEM_TYPE, DDL_DROP_UNIQUE_KEY);
        } else if (const_type == "F") {
            list_item = new QListWidgetItem("Create Foreign Key");
            list->addItem(list_item);
            list_item->setData(ROLE_ITEM_TYPE, DDL_CREATE_FOREIGN_KEY);

            list_item = new QListWidgetItem("Drop Foreign Key");
            list->addItem(list_item);
            list_item->setData(ROLE_ITEM_TYPE, DDL_DROP_FOREIGN_KEY);
        }
        break;
    case TRIGGERS_ITEM:
        list_item = new QListWidgetItem("Create Triggers");
        list->addItem(list_item);
        list_item->setData(ROLE_ITEM_TYPE, DDL_CREATE_TRIGGERS);

        list_item = new QListWidgetItem("Drop Triggers");
        list->addItem(list_item);
        list_item->setData(ROLE_ITEM_TYPE, DDL_DROP_TRIGGERS);

        list_item = new QListWidgetItem("Enable Triggers");
        list->addItem(list_item);
        list_item->setData(ROLE_ITEM_TYPE, DDL_ENABLE_TRIGGERS);

        list_item = new QListWidgetItem("Disable Triggers");
        list->addItem(list_item);
        list_item->setData(ROLE_ITEM_TYPE, DDL_DISABLE_TRIGGERS);
        break;
    case TRIGGER_ITEM:
        list_item = new QListWidgetItem("Create Trigger");
        list->addItem(list_item);
        list_item->setData(ROLE_ITEM_TYPE, DDL_CREATE_TRIGGER);

        list_item = new QListWidgetItem("Drop Trigger");
        list->addItem(list_item);
        list_item->setData(ROLE_ITEM_TYPE, DDL_DROP_TRIGGER);

        list_item = new QListWidgetItem("Enable Trigger");
        list->addItem(list_item);
        list_item->setData(ROLE_ITEM_TYPE, DDL_ENABLE_TRIGGER);

        list_item = new QListWidgetItem("Disable Trigger");
        list->addItem(list_item);
        list_item->setData(ROLE_ITEM_TYPE, DDL_DISABLE_TRIGGER);
        break;
    case TYPES_ITEM:
        list_item = new QListWidgetItem("Create Types");
        list->addItem(list_item);
        list_item->setData(ROLE_ITEM_TYPE, DDL_CREATE_TYPES);

        list_item = new QListWidgetItem("Drop Types");
        list->addItem(list_item);
        list_item->setData(ROLE_ITEM_TYPE, DDL_DROP_TYPES);
        break;
    case TYPE_ITEM:
        list_item = new QListWidgetItem("Create Type");
        list->addItem(list_item);
        list_item->setData(ROLE_ITEM_TYPE, DDL_CREATE_TYPE);

        list_item = new QListWidgetItem("Drop Type");
        list->addItem(list_item);
        list_item->setData(ROLE_ITEM_TYPE, DDL_DROP_TYPE);
        break;
    case VIEWS_ITEM:
        list_item = new QListWidgetItem("Create Views");
        list->addItem(list_item);
        list_item->setData(ROLE_ITEM_TYPE, DDL_CREATE_VIEWS);

        list_item = new QListWidgetItem("Drop Views");
        list->addItem(list_item);
        list_item->setData(ROLE_ITEM_TYPE, DDL_DROP_VIEWS);
        break;
    case VIEW_ITEM:
        list_item = new QListWidgetItem("Create View");
        list->addItem(list_item);
        list_item->setData(ROLE_ITEM_TYPE, DDL_CREATE_VIEW);

        list_item = new QListWidgetItem("Drop View");
        list->addItem(list_item);
        list_item->setData(ROLE_ITEM_TYPE, DDL_DROP_VIEW);

        list_item = new QListWidgetItem("View Owner");
        list->addItem(list_item);
        list_item->setData(ROLE_ITEM_TYPE, DDL_VIEW_OWNER);
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
    case PROCEDURES_ITEM:        
        if(item->childCount() == 0) processProcedures(item);
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
    case TABLE_COLUMNS_ITEM:
        if(item->childCount() == 0) processTableColumns(item);
        break;
    case TYPES_ITEM:
        if(item->childCount() == 0) processTypes(item);
        break;
    }
}

double DDLGenerationPlugin::getDatabaseVersion(PGconn *connection)
{
    QString resp;
    const char *sql =
            "SELECT (regexp_match(version(), '\\d+\\.\\d+?'))[1]  ";

    if (PQstatus(connection) == CONNECTION_OK) {
        PGresult *res = PQexec(connection, sql);

        if (PQresultStatus(res) != PGRES_TUPLES_OK)
        {
            PQclear(res);
            return 0.0;
        }

        int tuples = PQntuples(res);

        resp = QString::fromStdString(PQgetvalue(res, 0, 0));

        PQclear(res);

        return resp.toDouble();
    }

    return 0.0;
}

QString DDLGenerationPlugin::getDatabaseVersionString(PGconn *connection)
{
    QString resp;
    const char *sql =
            "select version()";

    if (PQstatus(connection) == CONNECTION_OK) {
        PGresult *res = PQexec(connection, sql);

        if (PQresultStatus(res) != PGRES_TUPLES_OK)
        {
            PQclear(res);
            return QString();
        }
        resp = QString::fromStdString(PQgetvalue(res, 0, 0));

        PQclear(res);

        return resp;
    }

    return QString();
}


QStringList DDLGenerationPlugin::createObjectList(PGconn *connection, const char *sql, int return_col, int param_count, ...)
{
    QStringList list;
    QString field_value;
    int next_pos;
    QString line;
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
            list << PQerrorMessage(connection);
            PQclear(res);
        } else {

            tuples = PQntuples(res);

            for (int i = 0; i < tuples; i++) {
                field_value = QString::fromStdString(PQgetvalue(res, i, return_col));
                next_pos = field_value.indexOf(QRegExp("\n|\r\n|\r"));
                if(next_pos == -1)
                    list << field_value;
                else {
                    while (next_pos !=-1) {
                        line = field_value.left(next_pos+1);
                        field_value = field_value.mid(next_pos+1);
                        list << line;
                        next_pos = field_value.indexOf(QRegExp("\n|\r\n|\r"));
                    }

                }
            }

            PQclear(res);
        }
    }

    return list;
}

PGresult *DDLGenerationPlugin::createObjectList(PGconn *connection, const char *sql, int param_count, ...)
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

QStringList DDLGenerationPlugin::formatOutputToTable(PGresult *res, QString title, bool comments)
{
    int tuples = PQntuples(res);
    int columns = PQnfields(res);
    QString line_separator = "";
    QList<QVariant> row;
    QList<QList<QVariant>> rows;
    QList<QString> fields;
    QList<int> max_field_lengths;
    QStringList resp;
    QString line;

    if(comments)
        resp << "/**\n";

    if (title.length() != 0) {
        resp << (comments ? "  " : "") + title + "\n";
    }

    // get field names
    for (int i = 0; i < columns; i++) {
        fields.append(QString::fromStdString(PQfname(res, i)));
        max_field_lengths.append(QString::fromStdString(PQfname(res, i)).length());
    }

    // get values
    for (int i = 0; i < tuples; i++)
    {
        row.clear();
        for (int j = 0; j < columns; j++) {
            QVariant value = QString::fromStdString(PQgetvalue(res, i, j));
            row.append(value);
        }
        rows.append(row);
    }

    for (int c = 0; c < fields.count(); c++) {
        for(int r = 0; r < rows.count(); r++) {
            if (max_field_lengths.at(c) < rows.at(r).at(c).toString().length())
                max_field_lengths.replace(c, rows.at(r).at(c).toString().length());
        }
    }

    line_separator += "+";
    for(int c = 0; c < fields.count(); c++) {
        QString rep = "-";
        line_separator += rep.repeated(max_field_lengths.at(c))+"+";
    }

    line_separator += "\n";

    resp << (comments ? "  " : "") + line_separator;

    line = (comments ? "  |" : "|");
    for(int c = 0; c < fields.count(); c++) {
        line += fields.at(c).leftJustified(max_field_lengths.at(c)) + "|";
    }
    line += "\n";
    resp << line;

    resp << (comments ? "  " : "") + line_separator;

    for (int r = 0; r < rows.count(); r++) {
        line = (comments ? "  |" : "|");
        for (int c = 0; c < rows.at(r).count(); c++) {
            line += rows.at(r).at(c).toString().leftJustified(max_field_lengths.at(c)) + "|";
        }
        line += "\n";
        resp << line;
    }

    resp << (comments ? "  " : "") + line_separator;

    if(comments)
        resp << "**/\n";

    return resp;
}

void DDLGenerationPlugin::processSchemas(QTreeWidgetItem *item) {

    PGconn *connection = trees[item->treeWidget()];
    double version = getDatabaseVersion(connection);

    QTreeWidgetItem *table_node;
    QTreeWidgetItem *view_node;
    QTreeWidgetItem *function_node;
    QTreeWidgetItem *procedure_node;
    QTreeWidgetItem *sequence_node;
    QTreeWidgetItem *trigger_function_node;
    QTreeWidgetItem *type_node;

    if (item->childCount() > 0)
        return;

    sequence_node = new QTreeWidgetItem();
    sequence_node->setText(0, "Sequences");
    sequence_node->setIcon(0, QIcon(":/icons/images/icons/sequences.png"));
    sequence_node->setData(0, ROLE_ITEM_TYPE, SEQUENCES_ITEM);
    sequence_node->setData(0, ROLE_SCHEMA_NAME, item->text(0));
    item->addChild(sequence_node);

    type_node = new QTreeWidgetItem();
    type_node->setText(0, "Types");
    type_node->setIcon(0, QIcon(":/icons/images/icons/types.png"));
    type_node->setData(0, ROLE_ITEM_TYPE, TYPES_ITEM);
    type_node->setData(0, ROLE_SCHEMA_NAME, item->text(0));
    item->addChild(type_node);

    function_node = new QTreeWidgetItem();
    function_node->setText(0, "Functions");
    function_node->setIcon(0, QIcon(":/icons/images/icons/functions.png"));
    function_node->setData(0, ROLE_ITEM_TYPE, FUNCTIONS_ITEM);
    function_node->setData(0, ROLE_SCHEMA_NAME, item->text(0));
    item->addChild(function_node);

    if (version >= 11) {

        // Procedures can be created only from Version 11

        procedure_node = new QTreeWidgetItem();
        procedure_node->setText(0, "Procedures");
        procedure_node->setIcon(0, QIcon(":/icons/images/icons/functions.png"));
        procedure_node->setData(0, ROLE_ITEM_TYPE, PROCEDURES_ITEM);
        procedure_node->setData(0, ROLE_SCHEMA_NAME, item->text(0));
        item->addChild(procedure_node);
    }

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
    QTreeWidgetItem *columns_node;
    QTreeWidgetItem *table;
    QStringList table_list;

    PGconn *connection = trees[item->treeWidget()];

    table_list = tables(connection, item->data(0, ROLE_SCHEMA_NAME).toString());

    for(int j=0; j < table_list.count(); j++) {

        table = new QTreeWidgetItem();
        table->setText(0, table_list[j]);
        table->setIcon(0, QIcon(":/icons/images/icons/table.png"));
        table->setData(0, Qt::UserRole, TABLE_ITEM);
        table->setData(0, ROLE_TABLE_NAME, table_list[j]);
        table->setData(0, ROLE_SCHEMA_NAME, item->data(0, ROLE_SCHEMA_NAME).toString());

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

        columns_node = new QTreeWidgetItem();
        columns_node->setText(0, "Columns");
        columns_node->setIcon(0, QIcon(":/icons/images/icons/columns.png"));
        columns_node->setData(0, ROLE_ITEM_TYPE, TABLE_COLUMNS_ITEM);
        columns_node->setData(0, ROLE_SCHEMA_NAME, item->data(0, ROLE_SCHEMA_NAME).toString());
        columns_node->setData(0, ROLE_TABLE_NAME, table_list[j]);
        table->addChild(columns_node);

    }
}

void DDLGenerationPlugin::processTypes(QTreeWidgetItem *item)
{
    QTreeWidgetItem *type;
    QStringList type_list;

    PGconn *connection = trees[item->treeWidget()];

    type_list = types(connection, item->data(0, ROLE_SCHEMA_NAME).toString());

    for(int j=0; j < type_list.count(); j++) {

        type = new QTreeWidgetItem();
        type->setText(0, type_list[j]);
        type->setIcon(0, QIcon(":/icons/images/icons/type.png"));
        type->setData(0, ROLE_SCHEMA_NAME, item->data(0, ROLE_SCHEMA_NAME).toString());
        type->setData(0, Qt::UserRole, TYPE_ITEM);        
        type->setData(0, ROLE_TYPE_NAME, type_list[j]);
        item->addChild(type);

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
        view->setData(0, ROLE_SCHEMA_NAME, item->data(0, ROLE_SCHEMA_NAME).toString());
        view->setData(0, ROLE_VIEW_NAME, view_list[j]);
        item->addChild(view);

        triggers_node = new QTreeWidgetItem();
        triggers_node->setText(0, "Triggers");
        triggers_node->setIcon(0, QIcon(":/icons/images/icons/triggers.png"));
        triggers_node->setData(0, ROLE_ITEM_TYPE, TRIGGERS_ITEM);
        triggers_node->setData(0, ROLE_SCHEMA_NAME, item->data(0, ROLE_SCHEMA_NAME).toString());
        triggers_node->setData(0, ROLE_VIEW_NAME, view_list[j]);
        view->addChild(triggers_node);

    }
}

void DDLGenerationPlugin::processSequences(QTreeWidgetItem *item)
{
    QStringList sequence_list;
    QTreeWidgetItem *sequence;
    QString schema = item->data(0, ROLE_SCHEMA_NAME).toString();

    PGconn *connection = trees[item->treeWidget()];

    sequence_list = sequences(connection, schema);

    for(int j=0; j < sequence_list.count(); j++) {

        sequence = new QTreeWidgetItem();
        sequence->setText(0, sequence_list[j]);
        sequence->setIcon(0, QIcon(":/icons/images/icons/sequence.png"));
        sequence->setData(0, ROLE_ITEM_TYPE, SEQUENCE_ITEM);
        sequence->setData(0, ROLE_SEQUENCE_NAME, sequence_list[j]);
        sequence->setData(0, ROLE_SCHEMA_NAME, schema);
        item->addChild(sequence);

    }
}

void DDLGenerationPlugin::processFunctions(QTreeWidgetItem *item)
{
    QStringList function_list;
    QTreeWidgetItem *function;
    QString schema;
    PGconn *connection = trees[item->treeWidget()];

    schema = item->data(0, ROLE_SCHEMA_NAME).toString();
    function_list = functions(connection, schema);

    for(int j=0; j < function_list.count(); j++) {

        function = new QTreeWidgetItem();
        function->setText(0, function_list[j]);
        function->setIcon(0, QIcon(":/icons/images/icons/function.png"));
        function->setData(0, ROLE_ITEM_TYPE, FUNCTION_ITEM);
        function->setData(0, ROLE_FUNCTION_NAME, function_list[j]);
        function->setData(0, ROLE_SCHEMA_NAME, schema);
        item->addChild(function);

    }
}

void DDLGenerationPlugin::processProcedures(QTreeWidgetItem *item)
{
    QStringList procedure_list;
    QTreeWidgetItem *procedure;
    QString schema;
    PGconn *connection = trees[item->treeWidget()];

    schema = item->data(0, ROLE_SCHEMA_NAME).toString();
    procedure_list = procedures(connection, schema);

    for(int j=0; j < procedure_list.count(); j++) {

        procedure = new QTreeWidgetItem();
        procedure->setText(0, procedure_list[j]);
        procedure->setIcon(0, QIcon(":/icons/images/icons/function.png"));
        procedure->setData(0, ROLE_ITEM_TYPE, PROCEDURE_ITEM);
        procedure->setData(0, ROLE_PROCEDURE_NAME, procedure_list[j]);
        procedure->setData(0, ROLE_SCHEMA_NAME, schema);
        item->addChild(procedure);

    }
}

void DDLGenerationPlugin::processTrigerFunctions(QTreeWidgetItem *item)
{
    QStringList function_list;
    QTreeWidgetItem *function;
    QString schema;

    PGconn *connection = trees[item->treeWidget()];

    schema = item->data(0, ROLE_SCHEMA_NAME).toString();
    function_list = triggerFunctions(connection, item->data(0, ROLE_SCHEMA_NAME).toString());

    for(int j=0; j < function_list.count(); j++) {

        function = new QTreeWidgetItem();
        function->setText(0, function_list[j]);
        function->setIcon(0, QIcon(":/icons/images/icons/trigger_function.png"));
        function->setData(0, ROLE_ITEM_TYPE, TRIGGER_FUNCTION_ITEM);
        function->setData(0, ROLE_FUNCTION_NAME, function_list[j]);
        function->setData(0, ROLE_SCHEMA_NAME, schema);
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
        constraint->setData(0, ROLE_ITEM_TYPE, CONSTRAINT_ITEM);
        constraint->setData(0, ROLE_TABLE_NAME, table);
        constraint->setData(0, ROLE_SCHEMA_NAME, schema);
        constraint->setData(0, ROLE_CONSTRAINT_NAME, constr[0]);
        constraint->setData(0, ROLE_CONSTRAINT_TYPE, "P");
        item->addChild(constraint);
    }

    // unique key
    constr = constraints(connection, schema, table, "u");
    for(int j=0; j < constr.count(); j++) {
        constraint = new QTreeWidgetItem();
        constraint->setText(0, constr[j]);
        constraint->setIcon(0, QIcon(":/icons/images/icons/unique_key.png"));
        constraint->setData(0, ROLE_ITEM_TYPE, CONSTRAINT_ITEM);
        constraint->setData(0, ROLE_TABLE_NAME, table);
        constraint->setData(0, ROLE_SCHEMA_NAME, schema);
        constraint->setData(0, ROLE_CONSTRAINT_NAME, constr[j]);
        constraint->setData(0, ROLE_CONSTRAINT_TYPE, "U");
        item->addChild(constraint);
    }

    // foreign key
    constr = constraints(connection, schema, table, "f");
    for(int j=0; j < constr.count(); j++) {
        constraint = new QTreeWidgetItem();
        constraint->setText(0, constr[j]);
        constraint->setIcon(0, QIcon(":/icons/images/icons/foreign_key.png"));
        constraint->setData(0, ROLE_ITEM_TYPE, CONSTRAINT_ITEM);
        constraint->setData(0, ROLE_TABLE_NAME, table);
        constraint->setData(0, ROLE_SCHEMA_NAME, schema);
        constraint->setData(0, ROLE_CONSTRAINT_NAME, constr[j]);
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
        constraint->setData(0, ROLE_CONSTRAINT_NAME, constr[j]);
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
        trigger->setData(0, ROLE_TRIGGER_NAME, trigger_list[j]);
        item->addChild(trigger);
    }

}

void DDLGenerationPlugin::processTableColumns(QTreeWidgetItem *item)
{
    QString schema = item->data(0, ROLE_SCHEMA_NAME).toString();
    QString table = item->data(0, ROLE_TABLE_NAME).toString();
    QStringList column_list;
    QTreeWidgetItem *column;
    QString column_name;

    PGconn *connection = trees[item->treeWidget()];

    column_list = table_columns(connection, schema, table);

    for(int j=0; j < column_list.count(); j++) {
        column = new QTreeWidgetItem();
        column_name = column_list[j];
        column->setText(0, column_name.mid(1));
        if (column_name[0] == QChar('*')) {
            column->setIcon(0, QIcon(":/icons/images/icons/column_inherited.png"));
        } else {
            column->setIcon(0, QIcon(":/icons/images/icons/column.png"));
        }

        column->setData(0, ROLE_ITEM_TYPE, TABLE_COLUMN_ITEM);
        column->setData(0, ROLE_TABLE_NAME, table);
        column->setData(0, ROLE_SCHEMA_NAME, schema);
        item->addChild(column);
    }
}

QStringList DDLGenerationPlugin::createAllSchemas(PGconn *connection)
{
    const char *sql =
        "SELECT "
        "    'CREATE SCHEMA IF NOT EXISTS ' || schema_name || ' AUTHORIZATION ' || schema_owner || E';\n' AS create_schema "
        "FROM information_schema.schemata "
        "WHERE schema_name NOT IN ('public', 'information_schema') AND schema_name !~ '^pg_' "
        "ORDER BY schema_name ";
    return createObjectList(connection, sql, 0, 0);
}

QStringList DDLGenerationPlugin::dropAllSchemas(PGconn *connection)
{
    const char *sql =
        "SELECT "
        "    'DROP SCHEMA IF EXISTS ' || schema_name || E';\n' AS create_schema "
        "FROM information_schema.schemata "
        "WHERE schema_name NOT IN ('public', 'information_schema') AND schema_name !~ '^pg_' "
        "ORDER BY schema_name ";
    return createObjectList(connection, sql, 0, 0);
}

QStringList DDLGenerationPlugin::createSchema(PGconn *connection, QString schema)
{
    const char *sql =
        "SELECT "
        "    'CREATE SCHEMA IF NOT EXISTS ' || schema_name || ' AUTHORIZATION ' || schema_owner || E';\n' AS create_schema "
        "FROM information_schema.schemata "
        "WHERE schema_name = $1 ";
    return createObjectList(connection, sql, 0, 1, schema.toStdString().c_str());
}

QStringList DDLGenerationPlugin::dropSchema(PGconn *connection, QString schema)
{
    const char *sql =
        "SELECT "
        "    'DROP SCHEMA IF EXISTS ' || schema_name || E';\n' AS create_schema "
        "FROM information_schema.schemata "
        "WHERE schema_name = $1 ";
    return createObjectList(connection, sql, 0, 1, schema.toStdString().c_str());
}

QStringList DDLGenerationPlugin::resetAllSequences(PGconn *connection)
{
    const char *sql =
        "SELECT 'ALTER SEQUENCE ' || sequence_schema || '.' || sequence_name || E' RESTART;\n' "
        "FROM information_schema.sequences "
        "ORDER BY sequence_schema, sequence_name ";
    return createObjectList(connection, sql, 0, 0);
}

QStringList DDLGenerationPlugin::updatetAllSequences(PGconn *connection)
{
    const char *sql =
        "DO "
        "$$ "
        "DECLARE "
        "    seq record; "
        "    val integer; "
        "BEGIN "
        "    DROP TABLE IF EXISTS sequences; "
        "    CREATE TEMPORARY TABLE sequences ON COMMIT DROP AS "
        "        SELECT DISTINCT "
        "            d.refobjid::regclass AS table_name, "
        "            c.relname, "
        "            n.nspname, "
        "            a.attname AS field_name, "
        "            pg_get_serial_sequence(d.refobjid::regclass::text, a.attname::text) AS sequence_name, "
        "            0 AS value_to_update "
        "        FROM  "
        "            pg_depend d "
        "            INNER JOIN pg_class c ON c.oid = d.refobjid "
        "            INNER JOIN pg_namespace n ON c.relnamespace = n.oid "
        "            INNER JOIN pg_attribute a ON a.attrelid = d.refobjid AND a.attnum = d.refobjsubid "
        "        WHERE "
        "            pg_get_serial_sequence(d.refobjid::regclass::text, a.attname::text) IS NOT NULL; "

        "    FOR seq IN "
        "        SELECT *  FROM sequences "
        "    LOOP "
        "        EXECUTE 'SELECT MAX(' || seq.field_name || ') FROM ' || seq.table_name INTO val; "
        "        UPDATE sequences SET "
        "            value_to_update = val "
        "        WHERE table_name = seq.table_name AND field_name = seq.field_name; "
        "    END LOOP; "
        "END; "
        "$$; "
        " "
        "SELECT "
        "    CASE "
        "        WHEN value_to_update IS NULL THEN 'ALTER SEQUENCE ' || sequence_name || E' RESTART;\n' "
        "        ELSE 'ALTER SEQUENCE ' || sequence_name || ' RESTART WITH ' || value_to_update || E';\n' "
        "    END "
        "FROM sequences "
        "ORDER BY sequence_name ";
    return createObjectList(connection, sql, 0, 0);
}

QStringList DDLGenerationPlugin::resetSequences(PGconn *connection, QString schema)
{
    const char *sql =
        "SELECT 'ALTER SEQUENCE ' || sequence_schema || '.' || sequence_name || E' RESTART;\n' "
        "FROM information_schema.sequences "
        "WHERE sequence_schema = $1 "
        "ORDER BY sequence_schema, sequence_name ";
    return createObjectList(connection, sql, 0, 1, schema.toStdString().c_str());
}

QStringList DDLGenerationPlugin::updateSequences(PGconn *connection, QString schema)
{
    QString sql =
        "DO "
        "$BODY$ "
        "DECLARE "
        "    seq record; "
        "    val integer; "
        "BEGIN "
        "    DROP TABLE IF EXISTS sequences; "
        "    CREATE TEMPORARY TABLE sequences ON COMMIT DROP AS "
        "        SELECT DISTINCT "
        "            d.refobjid::regclass AS table_name, "
        "            c.relname, "
        "            n.nspname, "
        "            a.attname AS field_name, "
        "            pg_get_serial_sequence(d.refobjid::regclass::text, a.attname::text) AS sequence_name, "
        "            0 AS value_to_update "
        "        FROM  "
        "            pg_depend d "
        "            INNER JOIN pg_class c ON c.oid = d.refobjid "
        "            INNER JOIN pg_namespace n ON c.relnamespace = n.oid "
        "            INNER JOIN pg_attribute a ON a.attrelid = d.refobjid AND a.attnum = d.refobjsubid "
        "        WHERE "
        "            pg_get_serial_sequence(d.refobjid::regclass::text, a.attname::text) IS NOT NULL  AND "
        "            n.nspname = '%1';"
        "    FOR seq IN "
        "        SELECT *  FROM sequences "
        "    LOOP "
        "        EXECUTE 'SELECT MAX(' || seq.field_name || ') FROM ' || seq.table_name INTO val; "
        "        UPDATE sequences SET "
        "            value_to_update = val "
        "        WHERE table_name = seq.table_name AND field_name = seq.field_name; "
        "    END LOOP; "
        "END; "
        "$BODY$; "
        " "
        "SELECT "
        "    CASE "
        "        WHEN value_to_update IS NULL THEN 'ALTER SEQUENCE ' || sequence_name || E' RESTART;\n' "
        "        ELSE 'ALTER SEQUENCE ' || sequence_name || ' RESTART WITH ' || value_to_update || E';\n' "
        "    END "
        "FROM sequences "
        "ORDER BY sequence_nane ";
    sql = sql.arg(schema);
    return createObjectList(connection, sql.toStdString().c_str(), 0, 0);
}

QStringList DDLGenerationPlugin::resetSequence(PGconn *connection, QString schema, QString sequence)
{
    const char *sql =
        "SELECT 'ALTER SEQUENCE ' || sequence_schema || '.' || sequence_name || E' RESTART;\n' "
        "FROM information_schema.sequences "
        "WHERE sequence_schema = $1 AND sequence_name = $2 ";
    return createObjectList(connection, sql, 0, 2, schema.toStdString().c_str(), sequence.toStdString().c_str());
}

QStringList DDLGenerationPlugin::updateSequence(PGconn *connection, QString schema, QString sequence)
{
    QString sql =
            QString (
                "DO "
                "$BODY$ "
                "DECLARE "
                "    seq record; "
                "    val integer; "
                "BEGIN "
                "    DROP TABLE IF EXISTS sequences; "
                "    CREATE TEMPORARY TABLE sequences ON COMMIT DROP AS "
                "        SELECT DISTINCT "
                "            d.refobjid::regclass AS table_name, "
                "            c.relname, "
                "            n.nspname, "
                "            a.attname AS field_name, "
                "            pg_get_serial_sequence(d.refobjid::regclass::text, a.attname::text) AS sequence_name, "
                "            0 AS value_to_update "
                "        FROM  "
                "            pg_depend d "
                "            INNER JOIN pg_class c ON c.oid = d.refobjid "
                "            INNER JOIN pg_namespace n ON c.relnamespace = n.oid "
                "            INNER JOIN pg_attribute a ON a.attrelid = d.refobjid AND a.attnum = d.refobjsubid "
                "        WHERE "
                "            pg_get_serial_sequence(d.refobjid::regclass::text, a.attname::text) IS NOT NULL; "
                "    FOR seq IN "
                "        SELECT *  FROM sequences "
                "    LOOP "
                "        EXECUTE 'SELECT MAX(' || seq.field_name || ') FROM ' || seq.table_name INTO val; "
                "        UPDATE sequences SET "
                "            value_to_update = val "
                "        WHERE table_name = seq.table_name AND field_name = seq.field_name; "
                "    END LOOP; "
                "END; "
                "$BODY$; "
                " "
                "SELECT "
                "    CASE "
                "        WHEN value_to_update IS NULL THEN 'ALTER SEQUENCE ' || sequence_name || E' RESTART;\n' "
                "        ELSE 'ALTER SEQUENCE ' || sequence_name || ' RESTART WITH ' || value_to_update || E';\n' "
                "    END "
                "FROM sequences "
                "WHERE sequence_name = '%1.%2' "
            ).arg(schema).arg(sequence);

    return createObjectList(connection, sql.toStdString().c_str(), 0, 0);
}

QStringList DDLGenerationPlugin::createConstraints(PGconn *connection, QString schema, QString table)
{
    const char *sql =
            "SELECT DISTINCT c.table_schema, c.table_name, t.constraint_name, t.constraint_type "
            "FROM information_schema.table_constraints t "
            "JOIN information_schema.key_column_usage c "
            "     ON c.constraint_name = t.constraint_name "
            "     AND c.constraint_schema = t.constraint_schema "
            "     AND c.constraint_name = t.constraint_name "
            "WHERE "
            "    c.table_schema = $1  AND "
            "    c.table_name = $2 "
            "ORDER BY  t.constraint_type, t.constraint_name ";
    int tuples;
    QStringList resp;
    char *constr_name;
    char *constr_type;

    PGresult *res = createObjectList(connection, sql, 2, schema.toStdString().c_str(),
                                     table.toStdString().c_str());

    if (PQresultStatus(res) != PGRES_TUPLES_OK)
    {
        resp << PQerrorMessage(connection);
        PQclear(res);
    } else {

        tuples = PQntuples(res);

        for (int i = 0; i < tuples; i++) {
            constr_name = PQgetvalue(res, i, 2);
            constr_type = PQgetvalue(res, i, 3);

            if (strcmp(constr_type, "PRIMARY KEY") == 0) {
                resp = resp + createPrimaryKey(connection, schema, table, QString(constr_name));
            } else if (strcmp(constr_type, "UNIQUE") == 0) {
                resp = resp + createUniqueKey(connection, schema, table, QString(constr_name));
            } else if (strcmp(constr_type, "FOREIGN KEY") == 0) {
                resp = resp + createForeignKey(connection, schema, table, QString(constr_name));
            }
        }

        PQclear(res);
    }

    return resp;
}

QStringList DDLGenerationPlugin::dropConstraints(PGconn *connection, QString schema, QString table)
{
    const char *sql =
            "SELECT DISTINCT c.table_schema, c.table_name, t.constraint_name, t.constraint_type "
            "FROM information_schema.table_constraints t "
            "JOIN information_schema.key_column_usage c "
            "     ON c.constraint_name = t.constraint_name "
            "     AND c.constraint_schema = t.constraint_schema "
            "     AND c.constraint_name = t.constraint_name "
            "WHERE "
            "    c.table_schema = $1  AND "
            "    c.table_name = $2 "
            "ORDER BY  t.constraint_type, t.constraint_name ";
    int tuples;
    QStringList resp;
    char *constr_name;
    char *constr_type;

    PGresult *res = createObjectList(connection, sql, 2, schema.toStdString().c_str(),
                                     table.toStdString().c_str());

    if (PQresultStatus(res) != PGRES_TUPLES_OK)
    {
        resp << PQerrorMessage(connection);
        PQclear(res);
    } else {

        tuples = PQntuples(res);

        for (int i = 0; i < tuples; i++) {
            constr_name = PQgetvalue(res, i, 2);
            constr_type = PQgetvalue(res, i, 3);

            if (strcmp(constr_type, "PRIMARY KEY") == 0) {
                resp = resp + dropPrimaryKey(connection, schema, table, QString(constr_name));
            } else if (strcmp(constr_type, "UNIQUE") == 0) {
                resp = resp + dropUniqueKey(connection, schema, table, QString(constr_name));
            } else if (strcmp(constr_type, "FOREIGN KEY") == 0) {
                resp = resp + dropForeignKey(connection, schema, table, QString(constr_name));
            }
        }

        PQclear(res);
    }

    return resp;
}

QStringList DDLGenerationPlugin::createPrimaryKey(PGconn *connection, QString schema, QString table, QString primary_key)
{
    const char *sql =
            "SELECT 'ALTER TABLE ' || c.table_schema || '.' || c.table_name || ' ADD CONSTRAINT ' || "
            "       t.constraint_name || ' PRIMARY KEY (' || string_agg(c.column_name::text,', ') || E');\n' "
            "FROM information_schema.table_constraints t "
            "JOIN information_schema.key_column_usage c "
            "     ON c.constraint_name = t.constraint_name "
            "     AND c.constraint_schema = t.constraint_schema "
            "     AND c.constraint_name = t.constraint_name "
            "WHERE "
            "    t.constraint_type = 'PRIMARY KEY' AND "
            "    c.table_schema = $1  AND "
            "    c.table_name = $2 AND "
            "    c.constraint_name = $3 "
            "GROUP BY "
            "    c.table_schema, "
            "    c.table_name, "
            "    t.constraint_name ";
    return createObjectList(connection, sql, 0, 3,
                            schema.toStdString().c_str(),
                            table.toStdString().c_str(),
                            primary_key.toStdString().c_str());
}

QStringList DDLGenerationPlugin::dropPrimaryKey(PGconn *connection, QString schema, QString table, QString primary_key)
{
    const char *sql =
            "select DISTINCT 'ALTER TABLE ' || c.table_schema || '.' || c.table_name || ' DROP CONSTRAINT ' || "
            "       t.constraint_name || E';\n' "
            "from information_schema.table_constraints t "
            "join information_schema.key_column_usage c "
            "     on c.constraint_name = t.constraint_name "
            "     and c.constraint_schema = t.constraint_schema "
            "     and c.constraint_name = t.constraint_name "
            "where "
            "    t.constraint_type = 'PRIMARY KEY' AND "
            "    c.table_schema = $1  AND "
            "    c.table_name = $2 AND "
            "    c.constraint_name = $3 ";
    return createObjectList(connection, sql, 0, 3,
                            schema.toStdString().c_str(),
                            table.toStdString().c_str(),
                            primary_key.toStdString().c_str());
}

QStringList DDLGenerationPlugin::createUniqueKey(PGconn *connection, QString schema, QString table, QString unique_key)
{
    const char *sql =
            "SELECT 'ALTER TABLE ' || c.table_schema || '.' || c.table_name || ' ADD CONSTRAINT ' || "
            "       t.constraint_name || ' UNIQUE (' || string_agg(c.column_name::text,', ') || E');\n' "
            "FROM information_schema.table_constraints t "
            "JOIN information_schema.key_column_usage c "
            "     ON c.constraint_name = t.constraint_name "
            "     AND c.constraint_schema = t.constraint_schema "
            "     AND c.constraint_name = t.constraint_name "
            "WHERE "
            "    t.constraint_type = 'UNIQUE' AND "
            "    c.table_schema = $1  AND "
            "    c.table_name = $2 AND "
            "    c.constraint_name = $3 "
            "GROUP BY "
            "    c.table_schema, "
            "    c.table_name, "
            "    t.constraint_name ";
    return createObjectList(connection, sql, 0, 3,
                            schema.toStdString().c_str(),
                            table.toStdString().c_str(),
                            unique_key.toStdString().c_str());
}

QStringList DDLGenerationPlugin::dropUniqueKey(PGconn *connection, QString schema, QString table, QString unique_key)
{
    const char *sql =
            "select DISTINCT 'ALTER TABLE ' || c.table_schema || '.' || c.table_name || ' DROP CONSTRAINT ' || "
            "       t.constraint_name || E';\n' "
            "from information_schema.table_constraints t "
            "join information_schema.key_column_usage c "
            "     on c.constraint_name = t.constraint_name "
            "     and c.constraint_schema = t.constraint_schema "
            "     and c.constraint_name = t.constraint_name "
            "where "
            "    t.constraint_type = 'UNIQUE' AND "
            "    c.table_schema = $1  AND "
            "    c.table_name = $2 AND "
            "    c.constraint_name = $3 ";
    return createObjectList(connection, sql, 0, 3,
                            schema.toStdString().c_str(),
                            table.toStdString().c_str(),
                            unique_key.toStdString().c_str());
}

QStringList DDLGenerationPlugin::createForeignKey(PGconn *connection, QString schema, QString table, QString foreign_key)
{
    const char *sql=
            "SELECT 'ALTER TABLE ' || c.table_schema || '.' ||c.table_name || E'\n    ADD CONSTRAINT ' || "
            "    c.constraint_name || E' FOREIGN KEY (' || string_agg(c.column_name, ', ') || E')\n' || "
            "    '    REFERENCES ' || "
            "    t2.table_schema || '.' || t2.table_name || "
            "    ("
            "       SELECT ' (' || string_agg(column_name,', ') || ')'  "
            "       FROM information_schema.key_column_usage "
            "       WHERE "
            "           table_name = t2.table_name "
            "           AND table_schema = r.unique_constraint_schema "
            "           AND constraint_name = r.unique_constraint_name "
            "       GROUP BY table_schema, table_name, constraint_name ) || E'\n' || "
            "       '    ON UPDATE ' || r.update_rule || ' ON DELETE ' || r.delete_rule || "
            "       ( "
            "           CASE "
            "               WHEN t.is_deferrable = 'YES' THEN 'E'\n    'DEFERRABLE ' "
            "               ELSE '' "
            "           END"
            "       ) || ("
            "           CASE "
            "               WHEN t.initially_deferred = 'YES' THEN 'INITIALLY IMEDIATE '  "
            "               ELSE '' "
            "           END "
            "       ) || E';\n'  "
            ""
            "FROM information_schema.table_constraints t "
            "JOIN information_schema.key_column_usage c "
            "          ON t.constraint_schema = c.constraint_schema "
            "          AND t.constraint_name = c.constraint_name "
            "JOIN information_schema.referential_constraints r "
            "          ON t.constraint_schema = r.constraint_schema "
            "          AND t.constraint_name = r.constraint_name "
            "JOIN information_schema.table_constraints t2 "
            "          ON r.unique_constraint_schema = t2.constraint_schema "
            "          AND r.unique_constraint_name = t2.constraint_name "
            "WHERE t.constraint_type = 'FOREIGN KEY' AND "
            "    c.table_schema = $1  AND "
            "    c.table_name = $2 AND "
            "    c.constraint_name = $3 "
            "GROUP BY "
            "    c.table_schema, "
            "    c.table_name, "
            "    t2.table_name, "
            "    t2.table_schema,"
            "    c.constraint_name, "
            "    r.unique_constraint_schema, "
            "    r.unique_constraint_name, "
            "    r.update_rule, "
            "    r.delete_rule, "
            "    t.is_deferrable, "
            "    t.initially_deferred "
            "ORDER BY c.table_schema, "
            "         c.table_name; ";
    return createObjectList(connection, sql, 0, 3,
                            schema.toStdString().c_str(),
                            table.toStdString().c_str(),
                            foreign_key.toStdString().c_str());

}

QStringList DDLGenerationPlugin::dropForeignKey(PGconn *connection, QString schema, QString table, QString foreign_key)
{
    const char *sql =
            "select DISTINCT 'ALTER TABLE ' || c.table_schema || '.' || c.table_name || ' DROP CONSTRAINT ' || "
            "       t.constraint_name || E';\n' "
            "from information_schema.table_constraints t "
            "join information_schema.key_column_usage c "
            "     on c.constraint_name = t.constraint_name "
            "     and c.constraint_schema = t.constraint_schema "
            "     and c.constraint_name = t.constraint_name "
            "where "
            "    t.constraint_type = 'FOREIGN KEY' AND "
            "    c.table_schema = $1  AND "
            "    c.table_name = $2 AND "
            "    c.constraint_name = $3 ";
    return createObjectList(connection, sql, 0, 3,
                            schema.toStdString().c_str(),
                            table.toStdString().c_str(),
                            foreign_key.toStdString().c_str());
}

QStringList DDLGenerationPlugin::createAllTriggers(PGconn *connection)
{
    const char *sql =
        "WITH "
        "    triggers as ( "
        "        SELECT "
        "            pg_get_triggerdef(trg.oid) AS create_trigger "
        "        FROM "
        "            pg_trigger trg "
        "            INNER JOIN pg_class tbl ON trg.tgrelid = tbl.oid "
        "            INNER JOIN pg_namespace n ON tbl.relnamespace = n.oid "
        "        WHERE "
        "            tbl.relname !~ '^pg_' "
        "            AND tgisinternal = FALSE "
        "    ) "
        "    SELECT "
        "        replace( "
        "            replace( "
        "                replace( "
        "                    replace( "
        "                        replace( "
        "                            replace( "
        "                                replace( "
        "                                    replace( "
        "                                        replace(create_trigger, 'BEFORE ', E'\n    BEFORE ' "
        "                                        ), 'AFTER', E'\n    AFTER' "
        "                                    ), 'ON ', E'\n    ON ' "
        "                                ), 'FROM', E'\n    FROM' "
        "                            ), 'NOT DEFERRABLE', E'\n    NOT DEFERRABLE' "
        "                        ), 'INITIALLY', E'\n    INITIALLY' "
        "                    ), 'FOR ', E'\n    FOR ' "
        "                ), 'EXECUTE', E'\n    EXECUTE' "
        "            ), 'WHEN ', E'\n    WHEN ' "
        "        ) || E';\n\n' AS create_trigger "
        "    FROM triggers t ";
    return createObjectList(connection, sql, 0, 0);
}

QStringList DDLGenerationPlugin::dropAllTriggers(PGconn *connection)
{
    const char *sql =
        "SELECT "
        "    'DROP TRIGGER ' || trg.tgname || ' ON ' || n.nspname || '.' || tbl.relname || E';\n' AS drop_trigger "
        "FROM "
        "    pg_trigger trg "
        "    INNER JOIN pg_class tbl ON trg.tgrelid = tbl.oid "
        "    INNER JOIN pg_namespace n ON tbl.relnamespace = n.oid "
        "WHERE "
        "    tbl.relname !~ '^pg_' "
        "    AND tgisinternal = FALSE ";
    return createObjectList(connection, sql, 0, 0);
}

QStringList DDLGenerationPlugin::enableAllTriggers(PGconn *connection)
{
    const char *sql =
        "SELECT  "
        "    'ALTER TABLE ' || n.nspname || '.' || tbl.relname || ' ENABLE TRIGGER ' || trg.tgname || E';\n' AS disable_trigger "
        "FROM "
        "    pg_trigger trg "
        "    INNER JOIN pg_class tbl ON trg.tgrelid = tbl.oid "
        "    INNER JOIN pg_namespace n ON tbl.relnamespace = n.oid "
        "WHERE "
        "    tbl.relname !~ '^pg_' "
        "    AND tgisinternal = FALSE ";
        return createObjectList(connection, sql, 0, 0);
}

QStringList DDLGenerationPlugin::disableAllTriggers(PGconn *connection)
{
    const char *sql =
        "SELECT  "
        "    'ALTER TABLE ' || n.nspname || '.' || tbl.relname || ' DISABLE TRIGGER ' || trg.tgname || E';\n' AS disable_trigger "
        "FROM "
        "    pg_trigger trg "
        "    INNER JOIN pg_class tbl ON trg.tgrelid = tbl.oid "
        "    INNER JOIN pg_namespace n ON tbl.relnamespace = n.oid "
        "WHERE "
        "    tbl.relname !~ '^pg_' "
        "    AND tgisinternal = FALSE ";
    return createObjectList(connection, sql, 0, 0);
}

QStringList DDLGenerationPlugin::createTriggers(PGconn *connection, QString schema, QString table)
{
    const char *sql =
        "WITH "
        "    triggers as ( "
        "        SELECT "
        "            pg_get_triggerdef(trg.oid) AS create_trigger "
        "        FROM "
        "            pg_trigger trg "
        "            INNER JOIN pg_class tbl ON trg.tgrelid = tbl.oid "
        "            INNER JOIN pg_namespace n ON tbl.relnamespace = n.oid "
        "        WHERE "
        "            tbl.relname !~ '^pg_' "
        "            AND tgisinternal = FALSE "
        "            AND n.nspname = $1 "
        "            AND tbl.relname = $2"
        "    ) "
        "    SELECT "
        "        replace( "
        "            replace( "
        "                replace( "
        "                    replace( "
        "                        replace( "
        "                            replace( "
        "                                replace( "
        "                                    replace( "
        "                                        replace(create_trigger, 'BEFORE ', E'\n    BEFORE ' "
        "                                        ), 'AFTER', E'\n    AFTER' "
        "                                    ), 'ON ', E'\n    ON ' "
        "                                ), 'FROM', E'\n    FROM' "
        "                            ), 'NOT DEFERRABLE', E'\n    NOT DEFERRABLE' "
        "                        ), 'INITIALLY', E'\n    INITIALLY' "
        "                    ), 'FOR ', E'\n    FOR ' "
        "                ), 'EXECUTE', E'\n    EXECUTE' "
        "            ), 'WHEN ', E'\n    WHEN ' "
        "        ) || E';\n\n' AS create_trigger "
        "    FROM triggers t ";
    return createObjectList(connection, sql, 0, 2, schema.toStdString().c_str(), table.toStdString().c_str());
}

QStringList DDLGenerationPlugin::dropTriggers(PGconn *connection, QString schema, QString table)
{
    const char *sql =
        "SELECT "
        "    'DROP TRIGGER IF EXISTS ' || trg.tgname || ' ON ' || n.nspname || '.' || tbl.relname || E';\n' AS drop_trigger "
        "FROM "
        "    pg_trigger trg "
        "    INNER JOIN pg_class tbl ON trg.tgrelid = tbl.oid "
        "    INNER JOIN pg_namespace n ON tbl.relnamespace = n.oid "
        "WHERE "
        "    tbl.relname !~ '^pg_' "
        "    AND tgisinternal = FALSE "
        "    AND n.nspname = $1 "
        "    AND tbl.relname = $2";
    return createObjectList(connection, sql, 0, 2, schema.toStdString().c_str(), table.toStdString().c_str());
}

QStringList DDLGenerationPlugin::enableTriggers(PGconn *connection, QString schema, QString table)
{
    const char *sql =
        "SELECT  "
        "    'ALTER TABLE ' || n.nspname || '.' || tbl.relname || ' ENABLE TRIGGER ' || trg.tgname || E';\n' AS disable_trigger "
        "FROM "
        "    pg_trigger trg "
        "    INNER JOIN pg_class tbl ON trg.tgrelid = tbl.oid "
        "    INNER JOIN pg_namespace n ON tbl.relnamespace = n.oid "
        "WHERE "
        "    tbl.relname !~ '^pg_' "
        "    AND tgisinternal = FALSE "
        "    AND n.nspname = $1 "
        "    AND tbl.relname = $2";
    return createObjectList(connection, sql, 0, 2, schema.toStdString().c_str(), table.toStdString().c_str());
}

QStringList DDLGenerationPlugin::disableTriggers(PGconn *connection, QString schema, QString table)
{
    const char *sql =
        "SELECT  "
        "    'ALTER TABLE ' || n.nspname || '.' || tbl.relname || ' DISABLE TRIGGER ' || trg.tgname || E';\n' AS disable_trigger "
        "FROM "
        "    pg_trigger trg "
        "    INNER JOIN pg_class tbl ON trg.tgrelid = tbl.oid "
        "    INNER JOIN pg_namespace n ON tbl.relnamespace = n.oid "
        "WHERE "
        "    tbl.relname !~ '^pg_' "
        "    AND tgisinternal = FALSE "
        "    AND n.nspname = $1 "
        "    AND tbl.relname = $2";
    return createObjectList(connection, sql, 0, 2, schema.toStdString().c_str(), table.toStdString().c_str());
}

QStringList DDLGenerationPlugin::createTrigger(PGconn *connection, QString schema, QString table, QString trigger_name)
{
    const char *sql =
        "WITH "
        "    triggers as ( "
        "        SELECT "
        "            pg_get_triggerdef(trg.oid) AS create_trigger "
        "        FROM "
        "            pg_trigger trg "
        "            INNER JOIN pg_class tbl ON trg.tgrelid = tbl.oid "
        "            INNER JOIN pg_namespace n ON tbl.relnamespace = n.oid "
        "        WHERE "
        "            tbl.relname !~ '^pg_' "
        "            AND tgisinternal = FALSE "
        "            AND n.nspname = $1 "
        "            AND tbl.relname = $2 "
        "            AND trg.tgname = $3 "
        "    ) "
        "    SELECT "
        "        replace( "
        "            replace( "
        "                replace( "
        "                    replace( "
        "                        replace( "
        "                            replace( "
        "                                replace( "
        "                                    replace( "
        "                                        replace(create_trigger, 'BEFORE ', E'\n    BEFORE ' "
        "                                        ), 'AFTER', E'\n    AFTER' "
        "                                    ), 'ON ', E'\n    ON ' "
        "                                ), 'FROM', E'\n    FROM' "
        "                            ), 'NOT DEFERRABLE', E'\n    NOT DEFERRABLE' "
        "                        ), 'INITIALLY', E'\n    INITIALLY' "
        "                    ), 'FOR ', E'\n    FOR ' "
        "                ), 'EXECUTE', E'\n    EXECUTE' "
        "            ), 'WHEN ', E'\n    WHEN ' "
        "        ) || E';\n\n' AS create_trigger "
        "    FROM triggers t ";
    return createObjectList(connection, sql, 0, 3, schema.toStdString().c_str(), table.toStdString().c_str(),
                            trigger_name.toStdString().c_str());
}

QStringList DDLGenerationPlugin::dropTrigger(PGconn *connection, QString schema, QString table, QString trigger_name)
{
    const char *sql =
        "SELECT "
        "    'DROP TRIGGER IF EXISTS ' || trg.tgname || ' ON ' || n.nspname || '.' || tbl.relname || E';\n' AS drop_trigger "
        "FROM "
        "    pg_trigger trg "
        "    INNER JOIN pg_class tbl ON trg.tgrelid = tbl.oid "
        "    INNER JOIN pg_namespace n ON tbl.relnamespace = n.oid "
        "WHERE "
        "    tbl.relname !~ '^pg_' "
        "    AND tgisinternal = FALSE "
        "    AND n.nspname = $1 "
        "    AND tbl.relname = $2"
        "    AND trg.tgname = $3 ";
    return createObjectList(connection, sql, 0, 3, schema.toStdString().c_str(), table.toStdString().c_str(),
                            trigger_name.toStdString().c_str());
}

QStringList DDLGenerationPlugin::enableTrigger(PGconn *connection, QString schema, QString table, QString trigger_name)
{
    const char *sql =
        "SELECT  "
        "    'ALTER TABLE ' || n.nspname || '.' || tbl.relname || ' ENABLE TRIGGER ' || trg.tgname || E';\n' AS disable_trigger "
        "FROM "
        "    pg_trigger trg "
        "    INNER JOIN pg_class tbl ON trg.tgrelid = tbl.oid "
        "    INNER JOIN pg_namespace n ON tbl.relnamespace = n.oid "
        "WHERE "
        "    tbl.relname !~ '^pg_' "
        "    AND tgisinternal = FALSE "
        "    AND n.nspname = $1 "
        "    AND tbl.relname = $2"
        "    AND trg.tgname = $3 ";
    return createObjectList(connection, sql, 0, 3, schema.toStdString().c_str(), table.toStdString().c_str(),
                            trigger_name.toStdString().c_str());
}

QStringList DDLGenerationPlugin::disableTrigger(PGconn *connection, QString schema, QString table, QString trigger_name)
{
    const char *sql =
        "SELECT  "
        "    'ALTER TABLE ' || n.nspname || '.' || tbl.relname || ' DISABLE TRIGGER ' || trg.tgname || E';\n' AS disable_trigger "
        "FROM "
        "    pg_trigger trg "
        "    INNER JOIN pg_class tbl ON trg.tgrelid = tbl.oid "
        "    INNER JOIN pg_namespace n ON tbl.relnamespace = n.oid "
        "WHERE "
        "    tbl.relname !~ '^pg_' "
        "    AND tgisinternal = FALSE "
        "    AND n.nspname = $1 "
        "    AND tbl.relname = $2"
        "    AND trg.tgname = $3 ";
    return createObjectList(connection, sql, 0, 3, schema.toStdString().c_str(), table.toStdString().c_str(),
                            trigger_name.toStdString().c_str());
}

QStringList DDLGenerationPlugin::createFunctions(PGconn *connection, QString schema, bool trigger)
{
    /* more complete than pg_get_functiondef */
    QString sql =
        QString(
        "WITH "
        "    func_def AS ( "
        "        SELECT "
        "            quote_ident(n.nspname) AS schema_name, "
        "            quote_ident(p.proname) AS function_name, "
        "            pg_catalog.pg_get_function_identity_arguments(p.oid) AS arguments, "
        "            CASE "
        "               WHEN proretset THEN 'SETOF ' || coalesce(nullif(quote_ident(tn.nspname),'pg_catalog') || '.', '') "
        "               ELSE coalesce(nullif(quote_ident(tn.nspname),'pg_catalog') || '.', '') "
        "            END AS return_schema, "
        "            CASE "
        "                WHEN t.typname = 'timestamp' THEN 'timestamp without time zone' "
        "                WHEN t.typname = 'timestampz' THEN 'timestamp with time zone' "
        "                ELSE quote_ident(t.typname) "
        "           END AS return_type, "
        "            l.lanname AS language_name, "
        "            CASE "
        "               WHEN proretset THEN 'COST ' || procost || E'\n' || 'ROWS ' || p.prorows || E';\n' "
        "               ELSE 'COST ' || p.procost || E';\n' "
        "            END AS estimated_cost_rows, "
        "            CASE "
        "                WHEN p.provolatile = 'v' THEN 'VOLATILE' "
        "                WHEN p.provolatile = 'i' THEN 'IMMUTABLE' "
        "                WHEN p.provolatile = 's' THEN 'STABLE' "
        "            END behavior, "
        "            CASE "
        "                WHEN p.prosecdef = TRUE THEN 'SECURITY DEFINER' "
        "                ELSE 'SECURITY INVOKER' "
        "            END AS security, "
        "            CASE "
        "                WHEN p.proisstrict THEN 'RETURNS NULL ON NULL INPUT' "
        "               ELSE 'CALLED ON NULL INPUT' "
        "            END AS null_parameters, "
        "            p.prosrc AS code "
        "        FROM   pg_catalog.pg_proc p "
        "           JOIN pg_catalog.pg_namespace n ON n.oid = p.pronamespace "
        "           JOIN pg_catalog.pg_type t ON p.prorettype = t.oid "
        "           JOIN pg_catalog.pg_namespace tn ON t.typnamespace = tn.oid "
        "           JOIN pg_catalog.pg_language l ON p.prolang = l.oid "
        "        WHERE "
        "           t.typname %1 'trigger' "
        "    ) "
        "    SELECT "
        "        'CREATE OR REPLACE FUNCTION ' || schema_name || '.' || function_name || '(' || arguments || E')\n' || "
        "        '   RETURNS ' || return_schema || return_type || E'\n' || "
        "        '   LANGUAGE ' || language_name || E'\n' || "
        "        E'AS \n' || "
        "        E'$BODY$' || "
        "        code || "
        "        E'$BODY$\n' || "
        "        behavior || E'\n' || security || E'\n' || null_parameters || E'\n' || "
        "        estimated_cost_rows || E'\n' AS definition "
        "    FROM func_def "
        "    WHERE "
        "        schema_name  = $1").arg(trigger ? "=" : "!=");

    return createObjectList(connection, sql.toStdString().c_str(), 0, 1, schema.toStdString().c_str());
}

QStringList DDLGenerationPlugin::createFunction(PGconn *connection, QString schema, QString func_name)
{
    /* more complete than pg_get_functiondef */
    double version = getDatabaseVersion(connection);

    const char *sql_gt_11 =
            "WITH "
            "    func_def AS ( "
            "        SELECT "
            "            quote_ident(n.nspname) AS schema_name, "
            "            quote_ident(p.proname) AS function_name, "
            "            pg_catalog.pg_get_function_identity_arguments(p.oid) AS arguments, "
            "            CASE "
            "               WHEN proretset THEN 'SETOF ' || coalesce(nullif(quote_ident(tn.nspname),'pg_catalog') || '.', '') "
            "               ELSE coalesce(nullif(quote_ident(tn.nspname),'pg_catalog') || '.', '') "
            "            END AS return_schema, "
            "            CASE "
            "                WHEN t.typname = 'timestamp' THEN 'timestamp without time zone' "
            "                WHEN t.typname = 'timestampz' THEN 'timestamp with time zone' "
            "                ELSE quote_ident(t.typname) "
            "           END AS return_type, "
            "            l.lanname AS language_name, "
            "            CASE "
            "               WHEN proretset THEN 'COST ' || procost || E'\n' || 'ROWS ' || p.prorows || E';\n' "
            "               ELSE 'COST ' || p.procost || E';\n' "
            "            END AS estimated_cost_rows, "
            "            CASE "
            "                WHEN p.provolatile = 'v' THEN 'VOLATILE' "
            "                WHEN p.provolatile = 'i' THEN 'IMMUTABLE' "
            "                WHEN p.provolatile = 's' THEN 'STABLE' "
            "            END behavior, "
            "            CASE "
            "                WHEN p.prosecdef = TRUE THEN 'SECURITY DEFINER' "
            "                ELSE 'SECURITY INVOKER' "
            "            END AS security, "
            "            CASE "
            "                WHEN p.proisstrict THEN 'RETURNS NULL ON NULL INPUT' "
            "               ELSE 'CALLED ON NULL INPUT' "
            "            END AS null_parameters, "
            "            p.prosrc AS code "
            "        FROM   pg_catalog.pg_proc p "
            "           JOIN pg_catalog.pg_namespace n ON n.oid = p.pronamespace "
            "           JOIN pg_catalog.pg_type t ON p.prorettype = t.oid "
            "           JOIN pg_catalog.pg_namespace tn ON t.typnamespace = tn.oid "
            "           JOIN pg_catalog.pg_language l ON p.prolang = l.oid "
            "    ) "
            "    SELECT "
            "        'CREATE OR REPLACE FUNCTION ' || schema_name || '.' || function_name || '(' || arguments || E')\n' || "
            "        '   RETURNS ' || return_schema || return_type || E'\n' || "
            "        '   LANGUAGE ' || language_name || E'\n' || "
            "        E'AS \n' || "
            "        E'$BODY$' || "
            "        code || "
            "        E'$BODY$\n' || "
            "        behavior || E'\n' || security || E'\n' || null_parameters || E'\n' || "
            "        estimated_cost_rows AS definition "
            "    FROM func_def "
            "    WHERE "
            "        schema_name  = $1 AND "
            "        function_name = $2 ";
    const char *sql_lt_11 =
            "WITH "
            "    func_def AS ( "
            "        SELECT "
            "            quote_ident(n.nspname) AS schema_name, "
            "            quote_ident(p.proname) AS function_name, "
            "            pg_catalog.pg_get_function_identity_arguments(p.oid) AS arguments, "
            "            CASE "
            "               WHEN proretset THEN 'SETOF ' || coalesce(nullif(quote_ident(tn.nspname),'pg_catalog') || '.', '') "
            "               ELSE coalesce(nullif(quote_ident(tn.nspname),'pg_catalog') || '.', '') "
            "            END AS return_schema, "
            "            CASE "
            "                WHEN t.typname = 'timestamp' THEN 'timestamp without time zone' "
            "                WHEN t.typname = 'timestampz' THEN 'timestamp with time zone' "
            "                ELSE quote_ident(t.typname) "
            "           END AS return_type, "
            "            l.lanname AS language_name, "
            "            CASE "
            "               WHEN proretset THEN 'COST ' || procost || E'\n' || 'ROWS ' || p.prorows || E';\n' "
            "               ELSE 'COST ' || p.procost || E';\n' "
            "            END AS estimated_cost_rows, "
            "            CASE "
            "                WHEN p.provolatile = 'v' THEN 'VOLATILE' "
            "                WHEN p.provolatile = 'i' THEN 'IMMUTABLE' "
            "                WHEN p.provolatile = 's' THEN 'STABLE' "
            "            END behavior, "
            "            CASE "
            "                WHEN p.prosecdef = TRUE THEN 'SECURITY DEFINER' "
            "                ELSE 'SECURITY INVOKER' "
            "            END AS security, "
            "            CASE "
            "                WHEN p.proisstrict THEN 'RETURNS NULL ON NULL INPUT' "
            "               ELSE 'CALLED ON NULL INPUT' "
            "            END AS null_parameters, "
            "            p.prosrc AS code "
            "        FROM   pg_catalog.pg_proc p "
            "           JOIN pg_catalog.pg_namespace n ON n.oid = p.pronamespace "
            "           JOIN pg_catalog.pg_type t ON p.prorettype = t.oid "
            "           JOIN pg_catalog.pg_namespace tn ON t.typnamespace = tn.oid "
            "           JOIN pg_catalog.pg_language l ON p.prolang = l.oid "
            "    ) "
            "    SELECT "
            "        'CREATE OR REPLACE FUNCTION ' || schema_name || '.' || function_name || '(' || arguments || E')\n' || "
            "        '   RETURNS ' || return_schema || return_type || E'\n' || "
            "        '   LANGUAGE ' || language_name || E'\n' || "
            "        E'AS \n' || "
            "        E'$BODY$' || "
            "        code || "
            "        E'$BODY$\n' || "
            "        behavior || E'\n' || security || E'\n' || null_parameters || E'\n' || "
            "        estimated_cost_rows AS definition "
            "    FROM func_def "
            "    WHERE "
            "        schema_name  = $1 AND "
            "        function_name = $2 ";
    if (version >= 11)
        return createObjectList(connection, sql_gt_11, 0, 2, schema.toStdString().c_str(), func_name.toStdString().c_str());
    else
        return createObjectList(connection, sql_lt_11, 0, 2, schema.toStdString().c_str(), func_name.toStdString().c_str());
}

QStringList DDLGenerationPlugin::dropFunctions(PGconn *connection, QString schema)
{
    double version = getDatabaseVersion(connection);
    const char *sql_gt_11 =
        "SELECT format(E'DROP %s %s;\n' "
        "            , CASE "
        "                 WHEN p.prokind = 'a' THEN 'AGGREGATE' "
        "                 WHEN p.prokind = 'f' THEN 'FUNCTION' "
        "                 WHEN p.prokind = 'p' THEN 'PROCEDURE' "
        "                 ELSE 'FUNCTION' "
        "               END "
        "            , p.oid::regprocedure "
        "             ) AS stmt "
        "FROM   "
        "   pg_catalog.pg_proc p "
        "   JOIN pg_catalog.pg_namespace n ON n.oid = p.pronamespace "
        "WHERE  quote_ident(n.nspname) = $1 "
        "ORDER  BY 1 ";

    const char *sql_lt_11 =
        "SELECT format(E'DROP %s %s;\n' "
        "            , CASE WHEN p.proisagg THEN 'AGGREGATE' ELSE 'FUNCTION' END "
        "            , p.oid::regprocedure "
        "             ) AS stmt "
        "FROM   "
        "   pg_catalog.pg_proc p "
        "   JOIN pg_catalog.pg_namespace n ON n.oid = p.pronamespace "
        "WHERE  quote_ident(n.nspname) = $1 "
        "ORDER  BY 1 ";
    if (version >= 11)
        return createObjectList(connection, sql_gt_11, 0, 1, schema.toStdString().c_str());
    else
        return createObjectList(connection, sql_lt_11, 0, 1, schema.toStdString().c_str());
}

QStringList DDLGenerationPlugin::dropFunction(PGconn *connection, QString schema, QString func_name)
{
    double version = getDatabaseVersion(connection);
    const char *sql_gt_11 =
            "SELECT format(E'DROP %s %s;\n' "
            "            , CASE "
            "                 WHEN p.prokind = 'a' THEN 'AGGREGATE' "
            "                 WHEN p.prokind = 'f' THEN 'FUNCTION' "
            "                 WHEN p.prokind = 'p' THEN 'PROCEDURE' "
            "                 ELSE 'FUNCTION' "
            "               END "
            "            , p.oid::regprocedure "
            "             ) AS stmt "
            "FROM   "
            "   pg_catalog.pg_proc p "
            "   JOIN pg_catalog.pg_namespace n ON n.oid = p.pronamespace "
            "WHERE  quote_ident(n.nspname) = $1 "
            "       AND quote_ident(p.proname) = $2";

    const char *sql_lt_11 =
        "SELECT format(E'DROP %s %s;\n' "
        "            , CASE WHEN p.proisagg THEN 'AGGREGATE' ELSE 'FUNCTION' END "
        "            , p.oid::regprocedure "
        "             ) AS stmt "
        "FROM   "
        "   pg_catalog.pg_proc p "
        "   JOIN pg_catalog.pg_namespace n ON n.oid = p.pronamespace "
        "WHERE  quote_ident(n.nspname) = $1 "
            "       AND quote_ident(p.proname) = $2";
    if (version >= 11)
        return createObjectList(connection, sql_gt_11, 0, 2, schema.toStdString().c_str(), func_name.toStdString().c_str());
    else
        return createObjectList(connection, sql_lt_11, 0, 2, schema.toStdString().c_str(), func_name.toStdString().c_str());
}

QStringList DDLGenerationPlugin::functionOwner(PGconn *connection, QString schema, QString func_name)
{
    double version = getDatabaseVersion(connection);
    const char *sql_gt_11 =
            "SELECT format(E'ALTER %s %s OWNER TO %s;\n' "
            "            , CASE "
            "                 WHEN p.prokind = 'a' THEN 'AGGREGATE' "
            "                 WHEN p.prokind = 'f' THEN 'FUNCTION' "
            "                 WHEN p.prokind = 'p' THEN 'PROCEDURE' "
            "                 ELSE 'FUNCTION' "
            "               END "
            "            , p.oid::regprocedure "
            "            , p.proowner::regrole "
            "             ) AS stmt "
            "FROM   "
            "   pg_catalog.pg_proc p "
            "   JOIN pg_catalog.pg_namespace n ON n.oid = p.pronamespace "
            "WHERE  quote_ident(n.nspname) = $1 "
            "       AND quote_ident(p.proname) = $2";

    const char *sql_lt_11 =
        "SELECT format(E'DROP %s %s OWNER TO %s;\n' "
        "            , CASE WHEN p.proisagg THEN 'AGGREGATE' ELSE 'FUNCTION' END "
        "            , p.oid::regprocedure "
        "            , p.proowner::regrole "
        "             ) AS stmt "
        "FROM   "
        "   pg_catalog.pg_proc p "
        "   JOIN pg_catalog.pg_namespace n ON n.oid = p.pronamespace "
        "WHERE  quote_ident(n.nspname) = $1 "
            "       AND quote_ident(p.proname) = $2";
    if (version >= 11)
        return createObjectList(connection, sql_gt_11, 0, 2, schema.toStdString().c_str(), func_name.toStdString().c_str());
    else
        return createObjectList(connection, sql_lt_11, 0, 2, schema.toStdString().c_str(), func_name.toStdString().c_str());
}

QStringList DDLGenerationPlugin::createProcedure(PGconn *connection, QString schema, QString proc_name)
{
    /* more complete than pg_get_functiondef */

    const char *sql_gt_11 =
            "WITH "
            "    func_def AS ( "
            "        SELECT "
            "            quote_ident(n.nspname) AS schema_name, "
            "            quote_ident(p.proname) AS function_name, "
            "            pg_catalog.pg_get_function_identity_arguments(p.oid) AS arguments, "
            "            CASE "
            "               WHEN proretset THEN 'SETOF ' || coalesce(nullif(quote_ident(tn.nspname),'pg_catalog') || '.', '') "
            "               ELSE coalesce(nullif(quote_ident(tn.nspname),'pg_catalog') || '.', '') "
            "            END AS return_schema, "
            "            CASE "
            "                WHEN t.typname = 'timestamp' THEN 'timestamp without time zone' "
            "                WHEN t.typname = 'timestampz' THEN 'timestamp with time zone' "
            "                ELSE quote_ident(t.typname) "
            "           END AS return_type, "
            "            l.lanname AS language_name, "
            "            CASE "
            "                WHEN p.prosecdef = TRUE THEN 'SECURITY DEFINER' "
            "                ELSE 'SECURITY INVOKER' "
            "            END AS security, "            
            "            p.prosrc AS code "
            "        FROM   pg_catalog.pg_proc p "
            "           JOIN pg_catalog.pg_namespace n ON n.oid = p.pronamespace "
            "           JOIN pg_catalog.pg_type t ON p.prorettype = t.oid "
            "           JOIN pg_catalog.pg_namespace tn ON t.typnamespace = tn.oid "
            "           JOIN pg_catalog.pg_language l ON p.prolang = l.oid "
            "    ) "
            "    SELECT "
            "        'CREATE OR REPLACE PROCEDURE ' || schema_name || '.' || function_name || '(' || arguments || E')\n' || "
            "        '   LANGUAGE ' || language_name || E'\n' || "
            "        E'AS \n' || "
            "        E'$BODY$' || "
            "        code || "
            "        E'$BODY$\n' || "
            "        security || E';\n' AS definition "
            "    FROM func_def "
            "    WHERE "
            "        schema_name  = $1 AND "
            "        function_name = $2 ";
    return createObjectList(connection, sql_gt_11, 0, 2, schema.toStdString().c_str(), proc_name.toStdString().c_str());
}

QStringList DDLGenerationPlugin::dropProcedure(PGconn *connection, QString schema, QString proc_name)
{
    const char *sql_gt_11 =
            "SELECT format(E'DROP PROCEDURE IF EXISTS %s;\n' "
            "            , p.oid::regprocedure "
            "             ) AS stmt "
            "FROM   "
            "   pg_catalog.pg_proc p "
            "   JOIN pg_catalog.pg_namespace n ON n.oid = p.pronamespace "
            "WHERE  quote_ident(n.nspname) = $1 "
            "       AND quote_ident(p.proname) = $2";
    return createObjectList(connection, sql_gt_11, 0, 2, schema.toStdString().c_str(), proc_name.toStdString().c_str());
}

QStringList DDLGenerationPlugin::procedureOwner(PGconn *connection, QString schema, QString proc_name)
{
    const char *sql_gt_11 =
            "SELECT format(E'ALTER PROCEDURE %s OWNER TO %s;\n' "
            "            , p.oid::regprocedure "
            "            , p.proowner::regrole "
            "             ) AS stmt "
            "FROM   "
            "   pg_catalog.pg_proc p "
            "   JOIN pg_catalog.pg_namespace n ON n.oid = p.pronamespace "
            "WHERE  quote_ident(n.nspname) = $1 "
            "       AND quote_ident(p.proname) = $2";
    return createObjectList(connection, sql_gt_11, 0, 2, schema.toStdString().c_str(), proc_name.toStdString().c_str());
}

QStringList DDLGenerationPlugin::createTable(PGconn *connection, QString schema, QString table_name)
{
    QStringList resp;
    QString colname;
    QString coltype;
    QString inherited;
    QStringList keys;
    int tuples;

    const char *sql_pk =
        "SELECT '   ,CONSTRAINT ' || "
        "       t.constraint_name || ' PRIMARY KEY (' || array_to_string(array_agg(c.column_name::text),', ') || E')\n' "
        "FROM information_schema.table_constraints t "
        "JOIN information_schema.key_column_usage c "
        "     ON c.constraint_name = t.constraint_name "
        "     AND c.constraint_schema = t.constraint_schema "
        "     AND c.constraint_name = t.constraint_name "
        "WHERE "
        "    t.constraint_type = 'PRIMARY KEY' AND "
        "    c.table_schema = $1  AND "
        "    c.table_name = $2 "
        "GROUP BY "
        "    c.table_schema, "
        "    c.table_name, "
        "    t.constraint_name ";

    const char *sql_uk =
        "SELECT '   ,CONSTRAINT ' || "
        "       t.constraint_name || ' UNIQUE (' || array_to_string(array_agg(c.column_name::text),', ') || E')\n' "
        "FROM information_schema.table_constraints t "
        "JOIN information_schema.key_column_usage c "
        "     ON c.constraint_name = t.constraint_name "
        "     AND c.constraint_schema = t.constraint_schema "
        "     AND c.constraint_name = t.constraint_name "
        "WHERE "
        "    t.constraint_type = 'UNIQUE' AND "
        "    c.table_schema = $1  AND "
        "    c.table_name = $2 "
        "GROUP BY "
        "    c.table_schema, "
        "    c.table_name, "
        "    t.constraint_name ";


    const char *sql_col =
            "SELECT "
            "    pg_catalog.format_type(a.atttypid, a.atttypmod) AS field_type,  "
            "    EXISTS ( "
            "      SELECT 1 "
            "      FROM pg_inherits AS i "
            "         JOIN pg_attribute AS a2 "
            "            ON i.inhparent = a2.attrelid "
            "         WHERE i.inhrelid = a.attrelid "
            "           AND a.attname = a2.attname "
            "   ) AS inherited, "
            "    a.attname "
            "FROM "
            "    pg_catalog.pg_attribute a "
            "    INNER JOIN pg_catalog.pg_class c ON a.attrelid = c.oid "
            "    INNER JOIN pg_catalog.pg_namespace n ON n.oid = c.relnamespace "
            "WHERE "
            "    a.attnum > 0 "
            "    AND NOT a.attisdropped "
            "    AND n.nspname = $1 "
            "    AND c.relname = $2 ";

    const char *sql =
        "SELECT 'CREATE TABLE ' || c.relname || '.' || c.relname || E'(\n' "
        "FROM pg_class c "
        "     INNER JOIN pg_namespace n ON c.relnamespace = n.oid "
        "WHERE "
        "   n.nspname = $1 "
        "   AND c.relname = $2 ";

    resp = createObjectList(connection, sql, 0, 2, schema.toStdString().c_str(), table_name.toStdString().c_str());

    PGresult *res = createObjectList(connection, sql_col, 2, schema.toStdString().c_str(),
                                     table_name.toStdString().c_str());

    if (PQresultStatus(res) != PGRES_TUPLES_OK)
    {
        resp << PQerrorMessage(connection);
        PQclear(res);
    } else {

        tuples = PQntuples(res);

        for (int i = 0; i < tuples; i++) {
            colname = QString::fromStdString(PQgetvalue(res, i, 2));
            coltype = QString::fromStdString(PQgetvalue(res, i, 0));
            inherited = QString::fromStdString(PQgetvalue(res, i, 1));
            if (inherited != "t")
                resp <<  (i == 0 ? "    ":"   ,") + colname + ' ' + coltype + "\n";
            else
                resp <<  (i == 0 ? "   -- ":"   -- ,") + colname + ' ' + coltype + "\n";
        }

        PQclear(res);
    }

    keys = createObjectList(connection, sql_pk, 0, 2,
                                     schema.toStdString().c_str(),
                                     table_name.toStdString().c_str());

    resp += keys;

    keys = createObjectList(connection, sql_uk, 0, 2,
                                     schema.toStdString().c_str(),
                                     table_name.toStdString().c_str());

    resp += keys;

    resp << ");\n";

    return resp;
}

QStringList DDLGenerationPlugin::createView(PGconn *connection, QString schema, QString view_name)
{
    const char *sql =
            "SELECT "
            "    'CREATE OR REPLACE VIEW ' || schemaname || '.' || viewname || E' AS \n' || "
            "     definition || E'\n' "
            "FROM pg_catalog.pg_views "
            "WHERE schemaname NOT IN ('pg_catalog', 'information_schema') AND "
            "      schemaname = $1 AND "
            "      viewname = $2 ";
    return createObjectList(connection, sql, 0, 2, schema.toStdString().c_str(), view_name.toStdString().c_str());
}

QStringList DDLGenerationPlugin::dropView(PGconn *connection, QString schema, QString view_name)
{
    const char *sql =
            "SELECT "
            "    'DROP VIEW IF EXISTS ' || schemaname || '.' || viewname || E';\n' "
            "FROM pg_catalog.pg_views "
            "WHERE schemaname NOT IN ('pg_catalog', 'information_schema') AND "
            "      schemaname = $1 AND "
            "      viewname = $2 ";
    return createObjectList(connection, sql, 0, 2, schema.toStdString().c_str(), view_name.toStdString().c_str());
}

QStringList DDLGenerationPlugin::viewOwner(PGconn *connection, QString schema, QString view_name)
{
    const char *sql =
            "SELECT "
            "    'ALTER VIEW ' || schemaname || '.' || viewname || ' OWNER TO ' || viewowner || E';\n' "
            "FROM pg_catalog.pg_views "
            "WHERE schemaname NOT IN ('pg_catalog', 'information_schema') AND "
            "      schemaname = $1 AND "
            "      viewname = $2 ";
    return createObjectList(connection, sql, 0, 2, schema.toStdString().c_str(), view_name.toStdString().c_str());
}

QStringList DDLGenerationPlugin::alterColumn(PGconn *connection, QString schema, QString column_name)
{
    /* TODO */
    return QStringList();
}

QStringList DDLGenerationPlugin::createAllTypes(PGconn *connection)
{
    const char *sql =
            "SELECT t.typname "
            "FROM "
            "    pg_type t "
            "    LEFT JOIN pg_catalog.pg_namespace n ON n.oid = t.typnamespace "
            "WHERE "
            "    ( "
            "        t.typrelid = 0 OR "
            "        ( "
            "            SELECT c.relkind = 'c' "
            "            FROM pg_catalog.pg_class c "
            "            WHERE c.oid = t.typrelid "
            "        ) "
            "    ) "
            "   AND NOT EXISTS ( "
            "       SELECT 1 "
            "       FROM pg_catalog.pg_type el "
            "       WHERE el.oid = t.typelem AND el.typarray = t.oid "
            "   ) "
            "   AND n.nspname NOT IN ('pg_catalog', 'information_schema') ";

    return createObjectList(connection, sql, 0, 0);
}

QStringList DDLGenerationPlugin::createTypes(PGconn *connection, QString schema)
{

}

QStringList DDLGenerationPlugin::createType(PGconn *connection, QString schema, QString type_name)
{
    QStringList resp;
    int tuples;
    int cols_tuples;
    QString t_type;
    QString t_input;
    QString t_output;
    QString t_receive;
    QString t_send;
    QString t_analyse;
    QString t_category;
    QString t_preferred;
    QString t_passed_by_val;
    QString t_type_default_bin;
    QString t_default;
    QString t_length;
    QString t_align;
    QString t_storage;

    QString t_col_name;
    QString t_col_data_type;

    const char *sql_cols =
            "WITH "
            "    types AS ( "
            "        SELECT n.nspname, "
            "                pg_catalog.format_type ( t.oid, NULL ) AS obj_name, "
            "                CASE "
            "                    WHEN t.typrelid != 0 THEN CAST ( 'tuple' AS pg_catalog.text ) "
            "                    WHEN t.typlen < 0 THEN CAST ( 'var' AS pg_catalog.text ) "
            "                    ELSE CAST ( t.typlen AS pg_catalog.text ) "
            "                    END AS obj_type, "
            "                coalesce ( pg_catalog.obj_description ( t.oid, 'pg_type' ), '' ) AS description "
            "            FROM pg_catalog.pg_type t "
            "            JOIN pg_catalog.pg_namespace n "
            "                ON n.oid = t.typnamespace "
            "            WHERE ( t.typrelid = 0 "
            "                    OR ( SELECT c.relkind = 'c' "
            "                            FROM pg_catalog.pg_class c "
            "                            WHERE c.oid = t.typrelid ) ) "
            "                AND NOT EXISTS ( "
            "                        SELECT 1 "
            "                            FROM pg_catalog.pg_type el "
            "                            WHERE el.oid = t.typelem "
            "                            AND el.typarray = t.oid ) "
            "                AND n.nspname <> 'pg_catalog' "
            "                AND n.nspname <> 'information_schema' "
            "                AND n.nspname !~ '^pg_toast' "
            "    ), "
            "    cols AS ( "
            "        SELECT n.nspname::text AS schema_name, "
            "                cl.relname, "
            "                a.attname::text AS column_name, "
            "                pg_catalog.format_type ( a.atttypid, a.atttypmod ) AS data_type, "
            "                a.attnotnull AS is_required, "
            "                a.attnum AS ordinal_position, "
            "                pg_catalog.col_description ( a.attrelid, a.attnum ) AS description "
            "            FROM pg_catalog.pg_attribute a "
            "            JOIN pg_catalog.pg_class cl ON cl.oid = a.attrelid "
            "            JOIN pg_catalog.pg_type t "
            "                ON a.attrelid = t.typrelid "
            "            JOIN pg_catalog.pg_namespace n "
            "                ON ( n.oid = t.typnamespace ) "
            "            JOIN types "
            "                ON ( types.nspname = n.nspname "
            "                    AND types.obj_name = pg_catalog.format_type ( t.oid, NULL ) ) "
            "            WHERE a.attnum > 0 "
            "                AND NOT a.attisdropped "
            "    ) "
            "SELECT cols.schema_name, "
            "        cols.relname, "
            "        cols.column_name, "
            "        cols.data_type, "
            "        cols.ordinal_position, "
            "        cols.is_required, "
            "        coalesce ( cols.description, '' ) AS description "
            "    FROM cols "
            "    WHERE schema_name = $1 AND relname = $2 "
            "    ORDER BY cols.schema_name, "            
            "             cols.relname, "
            "             cols.ordinal_position ";

    const char *sql =
        "SELECT"
        "   n.nspname, "
        "   t.typtype, "
        "   t.typinput, "
        "   t.typoutput, "
        "   t.typreceive, "
        "   t.typsend, "
        "   t.typanalyze, "
        "   t.typcategory, "
        "   t.typispreferred, "
        "   t.typbyval, "
        "   t.typdefaultbin, "
        "   t.typdefault, "
        "   t.typlen, "
        "   t.typalign, "
        "   t.typstorage "
        "FROM "
        "   pg_type t "
        "   LEFT JOIN pg_catalog.pg_namespace n ON n.oid = t.typnamespace "
        "WHERE "
        "   n.nspname = $1 "
        "   AND t.typname = $2 ";

    PGresult *cols;
    PGresult *res = createObjectList(connection, sql, 2, schema.toStdString().c_str(),
                                     type_name.toStdString().c_str());

    if (PQresultStatus(res) != PGRES_TUPLES_OK)
    {
        resp << PQerrorMessage(connection);
        PQclear(res);
    } else {

        tuples = PQntuples(res);

        if (tuples == 0)  {
            PQclear(res);
            return resp;
        }

        for (int i = 0; i < tuples; i++) {

            t_type = QString::fromStdString(PQgetvalue(res, i, 1));
            t_input = QString::fromStdString(PQgetvalue(res, i, 2));
            t_output = QString::fromStdString(PQgetvalue(res, i, 3));
            t_receive = QString::fromStdString(PQgetvalue(res, i, 4));
            t_send = QString::fromStdString(PQgetvalue(res, i, 5));
            t_analyse = QString::fromStdString(PQgetvalue(res, i, 6));
            t_category = QString::fromStdString(PQgetvalue(res, i, 7));
            t_preferred = QString::fromStdString(PQgetvalue(res, i, 8));
            if (t_preferred == "t")
                t_preferred = "TRUE";
            else
                t_preferred = "FALSE";
            t_passed_by_val = QString::fromStdString(PQgetvalue(res, i, 9));

            if (!PQgetisnull(res, i, 10)) {
                t_type_default_bin = QString::fromStdString(PQgetvalue(res, i, 10));
                t_default = QString::fromStdString(PQgetvalue(res, i, 11));
            } else {
                t_default = "''";
            }

            t_length = QString::fromStdString(PQgetvalue(res, i, 12));
            t_align = QString::fromStdString(PQgetvalue(res, i, 13));
            if (t_align == "c")
                t_align = "char";
            else if (t_align == "s")
                t_align = "int2";
            else if (t_align == "i")
                t_align = "int4";
            else if (t_align == "s")
                t_align = "double";

            t_storage = QString::fromStdString(PQgetvalue(res, i, 14));

            if (t_storage == "p")
                t_storage = "PLAIN";
            else if (t_storage == "e")
                t_storage = "EXTERNAL";
            else if (t_storage == "m")
                t_storage = "MAIN";
            else if (t_storage == "x")
                t_storage = "EXTENDED";
            /*
             * Check type to format the output
             *      b = base type;
             *      c = composit type;
             *      d = domain type;
             *      e = enum type;
             *      p = pseudo type;
             */
            if (t_type == 'c') {

                cols = createObjectList(connection, sql_cols, 2, schema.toStdString().c_str(),
                                                     type_name.toStdString().c_str());

                resp << "CREATE TYPE " + schema + '.' + type_name + " AS (\n\n";

                if (PQresultStatus(cols) != PGRES_TUPLES_OK)
                {
                    resp << PQerrorMessage(connection);
                    PQclear(cols);
                } else {

                    cols_tuples = PQntuples(cols);

                    if (cols_tuples == 0)  {
                        PQclear(cols);
                        return resp;
                    }

                    for (int i = 0; i < cols_tuples; i++) {

                        t_col_name = QString::fromStdString(PQgetvalue(cols, i, 2));
                        t_col_data_type = QString::fromStdString(PQgetvalue(cols, i, 3));
                        if (i ==0)
                            resp << "    " + t_col_name + " " +  t_col_data_type + "\n";
                        else
                            resp << "   ," + t_col_name + " " +  t_col_data_type + "\n";
                    }

                    resp << ");\n\n";

                    PQclear(cols);
                }


            } else if (t_type == "b") {
                resp << "CREATE TYPE " + type_name + "(\n";
                resp << "   INPUT=" + t_input + ",\n";
                resp << "   OUTPUT=" + t_output + "\n";
                resp << "   RECEIVE=" + t_receive + ",\n";
                resp << "   SEND=" + t_send + ",\n";
                resp << "   ANALYSE=" + t_analyse + ",\n";
                resp << "   CATEGORY=" + t_category + ",\n";
                resp << "   PREFERRED=" + t_preferred + ",\n";
                if (t_passed_by_val == "t") {
                    resp << "   PASSEDBYVALUE,\n";
                }
                resp << "   DEFAULT=" + t_default + ",\n";
                resp << "   INTERNAL LENGTH=" + t_length + ",\n";
                resp << "   ALIGNMENT=" + t_align + ",\n";
                resp << "   STORAGE=" + t_storage + "\n";
                resp << ");\n";

            }
        }

        PQclear(res);
    }

    return resp;
}


QStringList DDLGenerationPlugin::dropTypes(PGconn *connection, QString schema)
{
    const char *sql =
            "SELECT 'DROP TYPE IF EXISTS ' || n.nspname  || '.' || t.typname || E';\n' "
            "FROM "
            "    pg_type t "
            "    LEFT JOIN pg_catalog.pg_namespace n ON n.oid = t.typnamespace "
            "WHERE ( "
            "        t.typrelid = 0 OR ( "
            "            SELECT c.relkind = 'c' "
            "            FROM pg_catalog.pg_class c "
            "            WHERE c.oid = t.typrelid "
            "        ) "
            "    ) "
            "    AND NOT EXISTS ( "
            "        SELECT 1 "
            "        FROM pg_catalog.pg_type el "
            "        WHERE "
            "            el.oid = t.typelem AND el.typarray = t.oid "
            "    ) "
            "    AND n.nspname NOT IN ('pg_catalog', 'information_schema') "
            "    AND n.nspname = $1 ";
    return createObjectList(connection, sql, 0, 1, schema.toStdString().c_str());
}

QStringList DDLGenerationPlugin::dropType(PGconn *connection, QString schema, QString type_name)
{
    const char *sql =
            "SELECT 'DROP TYPE IF EXISTS ' || n.nspname  || '.' || t.typname || E';\n' "
            "FROM "
            "    pg_type t "
            "    LEFT JOIN pg_catalog.pg_namespace n ON n.oid = t.typnamespace "
            "WHERE ( "
            "        t.typrelid = 0 OR ( "
            "            SELECT c.relkind = 'c' "
            "            FROM pg_catalog.pg_class c "
            "            WHERE c.oid = t.typrelid "
            "        ) "
            "    ) "
            "    AND NOT EXISTS ( "
            "        SELECT 1 "
            "        FROM pg_catalog.pg_type el "
            "        WHERE "
            "            el.oid = t.typelem AND el.typarray = t.oid "
            "    ) "
            "    AND n.nspname NOT IN ('pg_catalog', 'information_schema') "
            "    AND n.nspname = $1 AND t.typname = $2 ";
    return createObjectList(connection, sql, 0, 2, schema.toStdString().c_str(), type_name.toStdString().c_str());
}

QStringList DDLGenerationPlugin::describeTableFields(PGconn *connection, QString schema, QString table)
{
    QStringList resp;
    double database_version = getDatabaseVersion(connection);
    PGresult *res;

    const char *sql_gt_91 =
        "SELECT "
        "    a.attname AS \"Field Name\", "
        "    pg_catalog.format_type(a.atttypid, a.atttypmod) AS \"Type\",  "
        "    EXISTS ( "
        "      SELECT 1 "
        "      FROM pg_inherits AS i "
        "         JOIN pg_attribute AS a2 "
        "            ON i.inhparent = a2.attrelid "
        "         WHERE i.inhrelid = a.attrelid "
        "           AND a.attname = a2.attname "
        "   ) AS \"Inherited\", "
        "   a.attnotnull AS \"Not Null\", "
        "   pg_get_expr(d.adbin, d.adrelid) AS \"Default\" "
        "FROM "
        "    pg_catalog.pg_attribute a "
        "    INNER JOIN pg_catalog.pg_class c ON a.attrelid = c.oid "
        "    INNER JOIN pg_catalog.pg_namespace n ON n.oid = c.relnamespace "
        "    LEFT  JOIN pg_attrdef d ON d.adrelid = a.attrelid AND d.adnum = a.attnum "
        "WHERE "
        "    a.attnum > 0 "
        "    AND NOT a.attisdropped "
        "    AND n.nspname = $1 "
        "    AND c.relname = $2 ";

    const char *sql_lt_91 =
        "SELECT "
        "    a.attname AS \"Field Name\", "
        "    pg_catalog.format_type(a.atttypid, a.atttypmod) AS \"Type\",  "
        "    EXISTS ( "
        "      SELECT 1 "
        "      FROM pg_inherits AS i "
        "         JOIN pg_attribute AS a2 "
        "            ON i.inhparent = a2.attrelid "
        "         WHERE i.inhrelid = a.attrelid "
        "           AND a.attname = a2.attname "
        "   ) AS \"Inherited\", "
        "   a.attnotnull AS \"Not Null\", "
        "   d.adsrc AS \"Default\" "
        "FROM "
        "    pg_catalog.pg_attribute a "
        "    INNER JOIN pg_catalog.pg_class c ON a.attrelid = c.oid "
        "    INNER JOIN pg_catalog.pg_namespace n ON n.oid = c.relnamespace "
        "    LEFT  JOIN pg_attrdef d ON d.adrelid = a.attrelid AND d.adnum = a.attnum "
        "WHERE "
        "    a.attnum > 0 "
        "    AND NOT a.attisdropped "
        "    AND n.nspname = $1 "
        "    AND c.relname = $2 ";

    if (database_version     >= 9.1)
         res = createObjectList(connection, sql_gt_91, 2, schema.toStdString().c_str(),
                                         table.toStdString().c_str());
    else
         res = createObjectList(connection, sql_lt_91, 2, schema.toStdString().c_str(),
                                         table.toStdString().c_str());
    resp = formatOutputToTable(res, QString("Table: %1.%2").arg(schema).arg(table), true);

    PQclear(res);

    return resp;
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
        "WHERE schema_name NOT IN ('information_schema') AND schema_name !~ '^pg_' "
        "ORDER BY schema_name ";
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
            "    AND schemaname = $1 "
            "ORDER BY tablename ";
    return createObjectList(connection, sql, 1, 1, schema.toStdString().c_str());
}

QStringList DDLGenerationPlugin::types(PGconn *connection, QString schema)
{
    const char *sql =
            "SELECT t.typname "
            "FROM "
            "    pg_type t "
            "    LEFT JOIN pg_catalog.pg_namespace n ON n.oid = t.typnamespace "
            "WHERE "
            "    ( "
            "        t.typrelid = 0 OR "
            "        ( "
            "            SELECT c.relkind = 'c' "
            "            FROM pg_catalog.pg_class c "
            "            WHERE c.oid = t.typrelid "
            "        ) "
            "    ) "
            "   AND NOT EXISTS ( "
            "       SELECT 1 "
            "       FROM pg_catalog.pg_type el "
            "       WHERE el.oid = t.typelem AND el.typarray = t.oid "
            "   ) "
            "   AND n.nspname = $1 "
            "ORDER BY t.typname ";

    /* AND n.nspname NOT IN ('pg_catalog', 'information_schema') */

    return createObjectList(connection, sql, 0, 1, schema.toStdString().c_str());
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
        "    AND schemaname = $1 "
        "ORDER BY viewname ";
    return createObjectList(connection, sql, 1, 1, schema.toStdString().c_str());
}

QStringList DDLGenerationPlugin::sequences(PGconn *connection, QString schema)
{
    const char *sql =
        "SELECT "
        "  sequence_schema, "
        "  sequence_name "
        "FROM information_schema.sequences "
        "WHERE sequence_schema = $1 "
        "ORDER BY sequence_name ";
    return createObjectList(connection, sql, 1, 1, schema.toStdString().c_str());
}

QStringList DDLGenerationPlugin::functions(PGconn *connection, QString schema)
{
    double database_version = getDatabaseVersion(connection);

    const char *sql_gt_11 =
        "SELECT "
        "    n.nspname, "
        "    p.proname, "
        "    pg_catalog.pg_get_function_identity_arguments(p.oid) as arguments, "
        "    t.typname "
        "FROM   pg_catalog.pg_proc p "
        "       JOIN pg_catalog.pg_namespace n ON n.oid = p.pronamespace "
        "       JOIN pg_catalog.pg_type t ON p.prorettype = t.oid "
        "WHERE "
        "    t.typname != 'trigger' AND p.prokind = 'f' AND "
        "    n.nspname = $1 "
        "ORDER BY p.proname ";
    const char *sql_lt_11 =
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
        "    n.nspname = $1 "
        "ORDER BY p.proname ";

    if (database_version >= 11)
        return createObjectList(connection, sql_gt_11, 1, 1, schema.toStdString().c_str());
    else
        return createObjectList(connection, sql_lt_11, 1, 1, schema.toStdString().c_str());
}

QStringList DDLGenerationPlugin::procedures(PGconn *connection, QString schema)
{
    double database_version = getDatabaseVersion(connection);

    const char *sql_gt_11 =
        "SELECT "
        "    n.nspname, "
        "    p.proname, "
        "    pg_catalog.pg_get_function_identity_arguments(p.oid) as arguments, "
        "    t.typname "
        "FROM   pg_catalog.pg_proc p "
        "       JOIN pg_catalog.pg_namespace n ON n.oid = p.pronamespace "
        "       JOIN pg_catalog.pg_type t ON p.prorettype = t.oid "
        "WHERE "
        "    t.typname != 'trigger' AND p.prokind = 'p' AND "
        "    n.nspname = $1 "
        "ORDER BY p.proname ";

    if (database_version >= 11)
        return createObjectList(connection, sql_gt_11, 1, 1, schema.toStdString().c_str());
    else
        return QStringList(); // there is no procedure before version 11

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
            "    n.nspname = $1 "
            "ORDER BY p.proname ";
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
    /* for detailed use split lines
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

QStringList DDLGenerationPlugin::table_columns(PGconn *connection, QString schema, QString table)
{
    const char *sql =
        "SELECT "
        "    CASE "
        "    WHEN EXISTS ( "
        "          SELECT 1 "
        "          FROM pg_inherits AS i "
        "             JOIN pg_attribute AS a2 "
        "                ON i.inhparent = a2.attrelid "
        "             WHERE i.inhrelid = a.attrelid "
        "               AND a.attname = a2.attname "
        "       ) THEN '*'"
        "     ELSE ' ' "
        "    END || "
        "    a.attname "
        "FROM "
        "    pg_catalog.pg_attribute a "
        "    INNER JOIN pg_catalog.pg_class c ON a.attrelid = c.oid "
        "    INNER JOIN pg_catalog.pg_namespace n ON n.oid = c.relnamespace "
        "WHERE "
        "    a.attnum > 0 "
        "    AND NOT a.attisdropped "
        "    AND n.nspname = $1 "
        "    AND c.relname = $2 ";
    return createObjectList(connection,
                            sql, 0, 2,
                            schema.toStdString().c_str(),
                            table.toStdString().c_str());
}

#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2(pgdba_ddlgenplugin, DDLGenerationPlugin)
#endif // QT_VERSION < 0x050000
