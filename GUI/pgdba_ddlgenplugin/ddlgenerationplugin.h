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
        DDL_CREATE_SCHEMA = 1000,
        DDL_DROP_SCHEMA,
        DDL_UPDATE_SEQUENCE,
        DDL_RESET_SEQUENCE,
        DDL_CREATE_TRIGGER,
        DDL_DROP_TRIGGER,
        DDL_ENABLE_TRIGGER,
        DDL_DISABLE_TRIGGER
    };
    DDLGenerationPlugin(QObject *parent = 0);
    void setMenu(QMenu *menu);
    void setTreeWidget(QTreeWidget *tree);

    bool run(PGconn *connection, int item, EditorItem *editor) override;

private:
    QString error;
    QString file_name;

    QStringList schemas(PGconn *connection);
    QStringList users(PGconn *connection);

    QString gen_create_schema(PGconn *connection, int offset);
    QString gen_drop_schema(PGconn *connection, int offset);
    QString gen_update_sequece(PGconn *connection, int offset);
    QString gen_reset_sequece(PGconn *connection, int offset);
    QString gen_create_trigger(PGconn *connection, int offset);
    QString gen_drop_trigger(PGconn *connection, int offset);
    QString gen_disable_trigger(PGconn *connection, int offset);
    QString gen_enable_trigger(PGconn *connection, int offset);
};

#endif // DDLGENERATIONPLUGIN_H
