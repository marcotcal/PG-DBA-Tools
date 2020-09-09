#ifndef PGDBAPLUGININTERFACE_H
#define PGDBAPLUGININTERFACE_H
#include <QString>
#include <libpq-fe.h>

#include <QString>
#include <libpq-fe.h>

/**
 * common interface for the SQL Generation
 */

class PGDBAPluginInterface
 {
 public:
    virtual ~PGDBAPluginInterface() {}
    virtual QString version() = 0;
    virtual QString plugin_name() = 0;
    virtual QString root_node_name() = 0;
    virtual QStringList itemNames() = 0;
    virtual QString fileName() = 0;
    virtual void setFileName(QString value) = 0;
    virtual bool run(PGconn *connection, QString item_name) = 0;
 };

#define PGDBAPluginInterface_iid "org.pgdbatools.PGDBAPluginInterface"

Q_DECLARE_INTERFACE(PGDBAPluginInterface, PGDBAPluginInterface_iid)

#endif // PGDBAPLUGININTERFACE_H
