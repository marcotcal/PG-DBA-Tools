#ifndef PGDBAPLUGININTERFACE_H
#define PGDBAPLUGININTERFACE_H
#include <QString>
#include <libpq-fe.h>

#include <QString>
#include <libpq-fe.h>
#include <sqltool.h>
#include <QMenu>
#include <QTreeWidget>
#include <QListWidget>

/**
 * common interface for the SQL Generation
 */

class PGDBAPluginInterface
 {
 public:
    virtual ~PGDBAPluginInterface() {}
    virtual void createTree(PGconn *connection, QTreeWidget *tree) = 0;
    virtual QStringList run(int item) = 0;

public slots:

    virtual void updateFunctionList(QTreeWidgetItem* item, QListWidget *list) = 0;

};

#define PGDBAPluginInterface_iid "org.pgdbatools.PGDBAPluginInterface"


Q_DECLARE_INTERFACE(PGDBAPluginInterface, PGDBAPluginInterface_iid)

#endif // PGDBAPLUGININTERFACE_H
