#ifndef CONNECTIONSETTINGS_H
#define CONNECTIONSETTINGS_H

#include <QObject>
#include <QMap>
#include <QList>
#include <QVariant>
#include <libpq-fe.h>

class ConnectionSettings : public QObject
{

    Q_OBJECT

public:
    enum {
        PL_BITMAP_SCAN = 1,
        PL_HASH_AGGREGATIONS = 2,
        PL_HASH_JOINS = 4,
        PL_INDEX_SCAN = 8,
        PL_INDEX_ONLY_SCAN = 16,
        PL_MATERIALIZATION = 32,
        PL_NESTED_LOOPS = 64,
        PL_SEQUENTIAL_SCAN = 128,
        PL_SORT_STEPS = 256,
        PL_TDI_SCAN = 512,
        PL_GATHER_MERGE = 1024,
        PL_MERGE_JOIN = 2048
    };

    enum {
        st_name,
        st_setting,
        st_unit,
        st_category,
        st_short_desc,
        st_extra_desc,
        st_context,
        st_vartype,
        st_source,
        st_min_val,
        st_max_val,
        st_enumvals,
        st_boot_val,
        st_reset_val,
        st_sourcefile,
        st_sourceline,
        st_pending_restart
    };

    ConnectionSettings(PGconn *conn, QObject *parent = nullptr);
    QVariant getSetting(QString setting, int field);

private:
    QMap<QString, QList<QVariant>> settings;
    QList<QString> columns;
    PGconn *conn;

    void readSettings();
};

#endif // CONNECTIONSETTINGS_H
