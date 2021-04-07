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
        ROLE_CONTRAINT_TYPE
    };
    DDLGenerationPlugin(QObject *parent = 0);
    void setMenu(QMenu *menu);
    void setTreeWidget(QTreeWidget *value);
    void setListWidget(QListWidget *value);
    void createTree(PGconn *value);
    bool run(PGconn *connection, int item, EditorItem *editor) override;

private slots:

    void processItem(QTreeWidgetItem *item, int column);
    void treeSelectionChanged();
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
