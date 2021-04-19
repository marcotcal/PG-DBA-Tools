#ifndef SQLGENERATIONPLUGIN_H
#define SQLGENERATIONPLUGIN_H

#include <QObject>
#include "pgdbaplugininterface.h"
#include <libpq-fe.h>
#include <Qsci/qsciscintilla.h>
#include <Qsci/qscilexersql.h>
#include <Qsci/qscilexer.h>
#include <sqltool.h>
#include <plugintreewidget.h>

class SQLGenerationPlugin : public QObject, PGDBAPluginInterface {
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.pgdbatools.PGDBAPluginInterface" FILE "pgdba_sqlgenplugin.json")
    Q_INTERFACES(PGDBAPluginInterface)

public:
    enum {
        SQL_TEST = 0,
        SQL_SELECT_FIRST_100 = 1000,
        SQL_SELECT_ALL,
        SQL_INSERT_ALL
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
    void createTree(PGconn *connection, QTreeWidget *tree) override;
    QStringList run(QTreeWidgetItem *tree_item, PGconn *connection, int command) override;

public slots:

    void updateFunctionList(QTreeWidgetItem* item,  QListWidget *list) override;

private slots:

    void processItem(QTreeWidgetItem *item, int column);

private:

    QMap<QTreeWidget *, PGconn *> trees;

    QStringList createObjectList(PGconn *connection, const char *sql, int return_col, int param_count, ...);
    PGresult *createObjectList(PGconn *connection, const char *sql, int param_count, ...);

    QStringList users(PGconn *connection);
    QStringList schemas(PGconn *connection);
    QStringList tables(PGconn *connection, QString schema);
    QStringList views(PGconn *connection, QString schema);
    QStringList functions(PGconn *connection, QString schema);

    QStringList insert_all(PGconn *connection, QString schema, QString table);

    void processSchemas(QTreeWidgetItem *item);
    void processTables(QTreeWidgetItem *item);
    void processViews(QTreeWidgetItem *item);
    void processFunctions(QTreeWidgetItem *item);

};

#endif // SQLGENERATIONPLUGIN_H
