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
    SQLGenerationPlugin(QObject *parent = 0);
    void setMenu(QMenu *menu);
    void setTreeWidget(QTreeWidget *value);
    void createTree(PGconn *connection);
    bool run(PGconn *connection, int item, EditorItem *editor) override;

private:
    QTreeWidget *tree;

    QString error;
    QString file_name;

    QStringList schemas(PGconn *connection);
    QStringList users(PGconn *connection);

    QString gen_insert_all(PGconn *connection, int offset);
    QString gen_insert_mandatory(PGconn *connection, int offset);

};

#endif // SQLGENERATIONPLUGIN_H
