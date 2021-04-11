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
        DDL_RESET_SEQUENCES,
        DDL_UPDATE_SEQUENCES,
        DDL_RESET_SEQUENCE,
        DDL_UPDATE_SEQUENCE
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
        ROLE_SCHEMA_NAME,
        ROLE_TABLE_NAME,
        ROLE_CONSTRAINT_TYPE
    };
    DDLGenerationPlugin(QObject *parent = 0);
    void setTreeWidget(QTreeWidget *value) override;
    void setListWidget(QListWidget *value) override;
    void createTree(PGconn *value) override;
    bool run(EditorItem *editor, int item) override;

    void generateFunctionList();

public slots:

    void updateFunctionList() override;

private slots:

    void processItem(QTreeWidgetItem *item, int column);
    void createAllSchemas();

private:

    QTreeWidget *tree;
    QListWidget *list;

    QString file_name;

    PGconn *connection;

    QStringList createObjectList(const char *sql, int return_col, int param_count, ...);

    QStringList users();
    QStringList schemas();
    QStringList tables(QString schema);
    QStringList views(QString schema);
    QStringList sequences(QString schema);
    QStringList functions(QString schema);
    QStringList triggerFunctions(QString schema);
    QStringList constraints(QString schema, QString table, char *ctype);
    QStringList triggers(QString schema, QString table);

    void processSchemas(QTreeWidgetItem *item);
    void processTables(QTreeWidgetItem *item);
    void processViews(QTreeWidgetItem *item);
    void processSequences(QTreeWidgetItem *item);
    void processFunctions(QTreeWidgetItem *item);
    void processTrigerFunctions(QTreeWidgetItem *item);
    void processConstraints(QTreeWidgetItem *item);
    void processTriggers(QTreeWidgetItem *item);

};

#endif // DDLGENERATIONPLUGIN_H
