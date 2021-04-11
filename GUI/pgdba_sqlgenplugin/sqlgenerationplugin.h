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
    void setMenu(QMenu *menu) override;
    void setTreeWidget(QTreeWidget *value) override;
    void setListWidget(QListWidget *value) override;
    void createTree(PGconn *connection) override;
    bool run(EditorItem *editor, int item) override;

private slots:

    void updateFunctionList() override;

private:
    QTreeWidget *tree;
    QListWidget *list;
    QString error;

    SqlTool *sql_tool;

    QString file_name;

    PGconn *connection;

    QStringList schemas(PGconn *connection);
    QStringList users(PGconn *connection);

    QString gen_insert_all(PGconn *connection, int offset);
    QString gen_insert_mandatory(PGconn *connection, int offset);

};

#endif // SQLGENERATIONPLUGIN_H
