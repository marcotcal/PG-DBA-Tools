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
    DDLGenerationPlugin(QObject *parent = 0);
    void setMenu(QMenu *menu);
    void setTreeWidget(QTreeWidget *tree);

    bool run(PGconn *connection, QString item_name, EditorItem *editor) override;

private:
    QString file_name;
};

#endif // DDLGENERATIONPLUGIN_H
