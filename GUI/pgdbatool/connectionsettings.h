#ifndef CONNECTIONSETTINGS_H
#define CONNECTIONSETTINGS_H

#include <QObject>
#include <QMap>
#include <QList>
#include <QVariant>
#include <libpq-fe.h>
#include <QException>

class ConnectionSettings : public QObject
{

    Q_OBJECT

public:

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
    QVariant getSetting(QString setting, int field=st_setting);
    bool compareSetting(QString setting, QVariant value);
    void alterSetting(QString setting, QVariant value);
private:
    QMap<QString, QList<QVariant>> settings;
    QList<QString> columns;
    PGconn *conn;

    void readSettings();
};

class SettingsException : public QException
{
public:
    SettingsException(QString const &message) : message(message) {}
    SettingsException *clone() const override;
    void raise() const override;
    QString getMessage() {
        return message;
    }
private:
    QString message;
};

#endif // CONNECTIONSETTINGS_H
