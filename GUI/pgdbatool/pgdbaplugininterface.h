#ifndef PGDBAPLUGININTERFACE_H
#define PGDBAPLUGININTERFACE_H
#include <QString>
#include <libpq-fe.h>

#include <QString>
#include <libpq-fe.h>
#include <sqltool.h>
#include <QMenu>
#include <QTreeWidget>

/**
 * common interface for the SQL Generation
 */

class PGDBAPluginInterface
 {
 public:
    virtual ~PGDBAPluginInterface() {}
    virtual void setMenu(QMenu *menu) = 0;
    virtual void setTreeWidget(QTreeWidget *tree) = 0;

    virtual bool run(PGconn *connection, QString item_name, EditorItem *editor) = 0;
 };

#define PGDBAPluginInterface_iid "org.pgdbatools.PGDBAPluginInterface"

Q_DECLARE_INTERFACE(PGDBAPluginInterface, PGDBAPluginInterface_iid)

#endif // PGDBAPLUGININTERFACE_H
