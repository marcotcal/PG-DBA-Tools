#ifndef PGDBAPLUGININTERFACE_H
#define PGDBAPLUGININTERFACE_H

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
 };

#define PGDBAPluginInterface_iid "org.pgdbatools.PGDBAPluginInterface"

Q_DECLARE_INTERFACE(PGDBAPluginInterface, PGDBAPluginInterface_iid)

#endif // PGDBAPLUGININTERFACE_H
