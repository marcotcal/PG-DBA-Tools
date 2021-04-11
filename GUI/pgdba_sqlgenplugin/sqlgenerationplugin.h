#ifndef SQLGENERATIONPLUGIN_H
#define SQLGENERATIONPLUGIN_H

#include <QObject>
#include "pgdbaplugininterface.h"
#include <libpq-fe.h>
#include <Qsci/qsciscintilla.h>
#include <Qsci/qscilexersql.h>
#include <Qsci/qscilexer.h>
#include <sqltool.h>

class SQLGenerationPlugin : public QObject, PGDBAPluginInterface {
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.pgdbatools.PGDBAPluginInterface" FILE "pgdba_sqlgenplugin.json")
    Q_INTERFACES(PGDBAPluginInterface)

public:
    enum {
        SQL_TEST = 0,
        SQL_INSERT_ALL = 1000,
        SQL_INSERT_MANDATORY
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
    };
    enum {
        ROLE_ITEM_TYPE = Qt::UserRole,
        ROLE_SCHEMA_NAME,
        ROLE_TABLE_NAME,
        ROLE_CONSTRAINT_TYPE
    };
    SQLGenerationPlugin(QObject *parent = 0);
    void setTreeWidget(QTreeWidget *value) override;
    void setListWidget(QListWidget *value) override;
    void createTree(PGconn *value) override;
    bool run(EditorItem *editor, int item) override;

    void generateFunctionList();

public slots:

    void updateFunctionList() override;

private slots:

    void processItem(QTreeWidgetItem *item, int column);


private:
    QTreeWidget *tree;
    QListWidget *list;

    QString error;

    QString file_name;

    PGconn *connection;

    QStringList createObjectList(const char *sql, int return_col, int param_count, ...);

    QStringList users();
    QStringList schemas();
    QStringList tables(QString schema);
    QStringList views(QString schema);
    QStringList functions(QString schema);

    QString gen_insert_all(PGconn *connection, int offset);
    QString gen_insert_mandatory(PGconn *connection, int offset);

    void processSchemas(QTreeWidgetItem *item);
    void processTables(QTreeWidgetItem *item);
    void processViews(QTreeWidgetItem *item);
    void processFunctions(QTreeWidgetItem *item);

};

#endif // SQLGENERATIONPLUGIN_H
