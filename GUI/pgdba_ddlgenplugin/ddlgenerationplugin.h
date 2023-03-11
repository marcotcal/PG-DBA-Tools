#ifndef DDLGENERATIONPLUGIN_H
#define DDLGENERATIONPLUGIN_H

#include <QObject>
#include "pgdbaplugininterface.h"
#include <libpq-fe.h>
#include <Qsci/qsciscintilla.h>
#include <Qsci/qscilexersql.h>
#include <Qsci/qscilexer.h>
#include <sqltool.h>

class DDLGenerationPlugin : public QObject, PGDBAPluginInterface {
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.pgdbatools.PGDBAPluginInterface" FILE "pgdba_ddlgenplugin.json")
    Q_INTERFACES(PGDBAPluginInterface)

public:
    enum {
        DDL_TEST = 0,
        DDL_VERSION,
        DDL_CREATE_ALL_SCHEMAS,
        DDL_DROP_ALL_SCHEMAS,
        DDL_CREATE_SCHEMA,
        DDL_DROP_SCHEMA,
        DDL_RESET_ALL_SEQUENCES,
        DDL_UPDATE_ALL_SEQUENCES,
        DDL_RESET_SEQUENCES,
        DDL_UPDATE_SEQUENCES,
        DDL_RESET_SEQUENCE,
        DDL_UPDATE_SEQUENCE,
        DDL_CREATE_ALL_TRIGGERS,
        DDL_DROP_ALL_TRIGGERS,
        DDL_ENABLE_ALL_TRIGGERS,
        DDL_DISABLE_ALL_TRIGGERS,
        DDL_CREATE_TRIGGERS,
        DDL_DROP_TRIGGERS,
        DDL_ENABLE_TRIGGERS,
        DDL_DISABLE_TRIGGERS,
        DDL_CREATE_TRIGGER,
        DDL_DROP_TRIGGER,
        DDL_ENABLE_TRIGGER,
        DDL_DISABLE_TRIGGER,
        DDL_CREATE_FUNCTIONS,
        DDL_CREATE_FUNCTION,        
        DDL_DROP_FUNCTIONS,
        DDL_DROP_FUNCTION,
        DDL_CREATE_PROCEDURES,
        DDL_CREATE_PROCEDURE,
        DDL_DROP_PROCEDURES,
        DDL_DROP_PROCEDURE,
        DDL_CREATE_TRIGGER_FUNCTIONS,
        DDL_CREATE_TABLE,
        DDL_CREATE_CONSTRAINTS,
        DDL_DROP_CONSTRAINTS,
        DDL_CREATE_PRIMARY_KEY,
        DDL_DROP_PRIMARY_KEY,
        DDL_CREATE_UNIQUE_KEY,
        DDL_DROP_UNIQUE_KEY,
        DDL_CREATE_FOREIGN_KEY,
        DDL_DROP_FOREIGN_KEY,
        DDL_CREATE_TYPES,
        DDL_DROP_TYPES,
        DDL_CREATE_TYPE,
        DDL_DROP_TYPE,
        DDL_DESCRIBE_TABLE_FIELDS,
        DDL_CREATE_VIEWS,
        DDL_DROP_VIEWS,
        DDL_VIEWS_OWNERS,
        DDL_CREATE_VIEW,
        DDL_DROP_VIEW,
        DDL_VIEW_OWNER,
    };
    enum {
        DATABASE_ITEM,
        SCHEMAS_ITEM,
        SCHEMA_ITEM,
        TABLES_ITEM,
        TABLE_ITEM,
        VIEWS_ITEM,
        VIEW_ITEM,
        SEQUENCES_ITEM,
        SEQUENCE_ITEM,
        FUNCTIONS_ITEM,
        FUNCTION_ITEM,
        PROCEDURES_ITEM,
        PROCEDURE_ITEM,
        TRIGGER_FUNCTIONS_ITEM,
        TRIGGER_FUNCTION_ITEM,
        CONSTRAINTS_ITEM,
        CONSTRAINT_ITEM,
        TRIGGERS_ITEM,
        TRIGGER_ITEM,
        TABLE_COLUMNS_ITEM,
        TABLE_COLUMN_ITEM,
        TYPES_ITEM,
        TYPE_ITEM
    };
    enum {
        ROLE_ITEM_TYPE = Qt::UserRole,
        ROLE_CONNECTION,
        ROLE_SCHEMA_NAME,
        ROLE_TABLE_NAME,
        ROLE_SEQUENCE_NAME,
        ROLE_CONSTRAINT_TYPE,
        ROLE_FUNCTION_NAME,
        ROLE_PROCEDURE_NAME,
        ROLE_TABLE_COLUMN_NAME,
        ROLE_CONSTRAINT_NAME,
        ROLE_TRIGGER_NAME,
        ROLE_TYPE_NAME,
        ROLE_VIEW_NAME
    };
    DDLGenerationPlugin(QObject *parent = 0);
    void createTree(PGconn *connection, QTreeWidget *tree) override;
    QStringList run(QTreeWidgetItem *tree_item, PGconn *connection, int command) override;

public slots:

    void updateFunctionList(QTreeWidgetItem* item,  QListWidget *list) override;

private slots:

    void processItem(QTreeWidgetItem *item, int column);

private:

    QMap<QTreeWidget *, PGconn *> trees;

    double getDatabaseVersion(PGconn *connection);
    QString getDatabaseVersionString(PGconn *connection);

    QStringList createObjectList(PGconn *connection, const char *sql, int return_col, int param_count, ...);
    PGresult *createObjectList(PGconn *connection, const char *sql, int param_count, ...);
    QStringList formatOutputToTable(PGresult *res, QString title, bool comments = true);
    QStringList users(PGconn *connection);
    QStringList schemas(PGconn *connection);
    QStringList tables(PGconn *connection, QString schema);
    QStringList types(PGconn *connection, QString schema);
    QStringList views(PGconn *connection, QString schema);
    QStringList sequences(PGconn *connection, QString schema);
    QStringList functions(PGconn *connection, QString schema);
    QStringList procedures(PGconn *connection, QString schema);
    QStringList triggerFunctions(PGconn *connection, QString schema);
    QStringList constraints(PGconn *connection, QString schema, QString table, char *ctype);
    QStringList triggers(PGconn *connection, QString schema, QString table);
    QStringList table_columns(PGconn *connection, QString schema, QString table);

    void processSchemas(QTreeWidgetItem *item);
    void processTables(QTreeWidgetItem *item);
    void processTypes(QTreeWidgetItem *item);
    void processViews(QTreeWidgetItem *item);
    void processSequences(QTreeWidgetItem *item);
    void processFunctions(QTreeWidgetItem *item);
    void processProcedures(QTreeWidgetItem *item);
    void processTrigerFunctions(QTreeWidgetItem *item);
    void processConstraints(QTreeWidgetItem *item);
    void processTriggers(QTreeWidgetItem *item);
    void processTableColumns(QTreeWidgetItem *item);

    QStringList createAllSchemas(PGconn *connection);
    QStringList dropAllSchemas(PGconn *connection);
    QStringList createSchema(PGconn *connection, QString schema);
    QStringList dropSchema(PGconn *connection, QString schema);

    QStringList resetAllSequences(PGconn *connection);
    QStringList updatetAllSequences(PGconn *connection);
    QStringList resetSequences(PGconn *connection, QString schema);
    QStringList updateSequences(PGconn *connection, QString schema);
    QStringList resetSequence(PGconn *connection, QString schema, QString sequence);
    QStringList updateSequence(PGconn *connection, QString schema,QString sequence);

    QStringList createConstraints(PGconn *connection, QString schema, QString table);
    QStringList dropConstraints(PGconn *connection, QString schema, QString table);

    QStringList createPrimaryKey(PGconn *connection, QString schema, QString table, QString primary_key);
    QStringList dropPrimaryKey(PGconn *connection, QString schema, QString table, QString primary_key);
    QStringList createUniqueKey(PGconn *connection, QString schema, QString table, QString unique_key);
    QStringList dropUniqueKey(PGconn *connection, QString schema, QString table, QString unique_key);
    QStringList createForeignKey(PGconn *connection, QString schema, QString table, QString foreign_key);
    QStringList dropForeignKey(PGconn *connection, QString schema, QString table, QString foreign_key);

    QStringList createAllTriggers(PGconn *connection);
    QStringList dropAllTriggers(PGconn *connection);
    QStringList enableAllTriggers(PGconn *connection);
    QStringList disableAllTriggers(PGconn *connection);

    QStringList createTriggers(PGconn *connection, QString schema, QString table);
    QStringList dropTriggers(PGconn *connection, QString schema, QString table);
    QStringList enableTriggers(PGconn *connection, QString schema, QString table);
    QStringList disableTriggers(PGconn *connection, QString schema, QString table);

    QStringList createTrigger(PGconn *connection, QString schema, QString table, QString trigger_name);
    QStringList dropTrigger(PGconn *connection, QString schema, QString table, QString trigger_name);
    QStringList enableTrigger(PGconn *connection, QString schema, QString table, QString trigger_name);
    QStringList disableTrigger(PGconn *connection, QString schema, QString table, QString trigger_name);

    QStringList createFunctions(PGconn *connection, QString schema, bool trigger);
    QStringList createFunction(PGconn *connection, QString schema, QString func_name);
    QStringList dropFunctions(PGconn *connection, QString schema);
    QStringList dropFunction(PGconn *connection, QString schema, QString func_name);

    QStringList createProcedure(PGconn *connection, QString schema, QString proc_name);
    QStringList dropProcedure(PGconn *connection, QString schema, QString proc_name);

    QStringList createTable(PGconn *connection, QString schema, QString table_name);

    QStringList createView(PGconn *connection, QString schema, QString view_name);
    QStringList dropView(PGconn *connection, QString schema, QString view_name);
    QStringList viewOwner(PGconn *connection, QString schema, QString view_name);

    QStringList alterColumn(PGconn *connection, QString schema, QString column_name);

    QStringList createAllTypes(PGconn *connection);

    QStringList createTypes(PGconn *connection, QString schema);
    QStringList createType(PGconn *connection, QString schema, QString type_name);

    QStringList dropTypes(PGconn *connection, QString schema);
    QStringList dropType(PGconn *connection, QString schema, QString type_name);

    QStringList describeTableFields(PGconn *connection, QString schema, QString table);

};

#endif // DDLGENERATIONPLUGIN_H
