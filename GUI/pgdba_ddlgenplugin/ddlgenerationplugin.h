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
        DDL_CREATE_FUNCTION
    };
    enum {
        DATABAE_ITEM,
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
        TRIGGER_FUNCTIONS_ITEM,
        TRIGGER_FUNCTION_ITEM,
        CONSTRAINTS_ITEM,
        TRIGGERS_ITEM,
        TRIGGER_ITEM
    };
    enum {
        ROLE_ITEM_TYPE = Qt::UserRole,
        ROLE_CONNECTION,
        ROLE_SCHEMA_NAME,
        ROLE_TABLE_NAME,
        ROLE_SEQUENCE_NAME,
        ROLE_CONSTRAINT_TYPE,
        ROLE_FUNCTION_NAME
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

    QStringList createObjectList(PGconn *connection, const char *sql, int return_col, int param_count, ...);

    QStringList users(PGconn *connection);
    QStringList schemas(PGconn *connection);
    QStringList tables(PGconn *connection, QString schema);
    QStringList views(PGconn *connection, QString schema);
    QStringList sequences(PGconn *connection, QString schema);
    QStringList functions(PGconn *connection, QString schema);
    QStringList triggerFunctions(PGconn *connection, QString schema);
    QStringList constraints(PGconn *connection, QString schema, QString table, char *ctype);
    QStringList triggers(PGconn *connection, QString schema, QString table);

    void processSchemas(QTreeWidgetItem *item);
    void processTables(QTreeWidgetItem *item);
    void processViews(QTreeWidgetItem *item);
    void processSequences(QTreeWidgetItem *item);
    void processFunctions(QTreeWidgetItem *item);
    void processTrigerFunctions(QTreeWidgetItem *item);
    void processConstraints(QTreeWidgetItem *item);
    void processTriggers(QTreeWidgetItem *item);

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

    QStringList createAllTriggers(PGconn *connection);
    QStringList dropAllTriggers(PGconn *connection);
    QStringList enableAllTriggers(PGconn *connection);
    QStringList disableAllTriggers(PGconn *connection);

    QStringList createFunction(PGconn *connection, QString schema, QString func_name);


};

#endif // DDLGENERATIONPLUGIN_H
