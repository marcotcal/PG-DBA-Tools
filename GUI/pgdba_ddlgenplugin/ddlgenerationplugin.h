#ifndef DDLGENERATIONPLUGIN_H
#define DDLGENERATIONPLUGIN_H

#include <QObject>
#include "pgdbaplugininterface.h"
#include <libpq-fe.h>

class DDLGenerationPlugin : public QObject, PGDBAPluginInterface {
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.pgdbatools.DDLGenerationPlugin" FILE "pgdba_ddlgenplugin.json")
    Q_INTERFACES(PGDBAPluginInterface)

public:
    DDLGenerationPlugin(QObject *parent = 0);
    QString version() override;
    QString plugin_name() override;
    QString root_node_name() override;
    QStringList itemNames() override;
    bool run(PGconn *connection, QString item_name) override;
};

#endif // DDLGENERATIONPLUGIN_H
