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
        SCHEMA_ITEM,
        TABLE_ITEM,
        VIEW_ITEM
    };
    DDLGenerationPlugin(QObject *parent = 0);
    void setMenu(QMenu *menu);
    void setTreeWidget(QTreeWidget *value);
    void createTree(PGconn *value);
    bool run(PGconn *connection, int item, EditorItem *editor) override;

private slots:

    void processItem(QTreeWidgetItem *item, int column);

private:

    QTreeWidget *tree;
    QString file_name;

    PGconn *connection;
    QStringList schemas(PGconn *connection);
    QStringList tables(QString schema, PGconn *connection);
    QStringList views(QString schema, PGconn *connection);
    QStringList users(PGconn *connection);

    void processSchema(QTreeWidgetItem *item);
    void processTable(QTreeWidgetItem *item);

};

#endif // DDLGENERATIONPLUGIN_H
