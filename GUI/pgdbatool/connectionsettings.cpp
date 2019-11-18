#include "connectionsettings.h"
#include <math.h>

ConnectionSettings::ConnectionSettings(PGconn *conn, QObject *parent) :
    QObject(parent),
    conn(conn)
{
    readSettings();
}

QVariant ConnectionSettings::getSetting(QString setting, int field)
{
    if (settings.contains(setting)) {
        return settings[setting][field];
    } else {
        return QVariant();
    }
}

bool ConnectionSettings::compareSetting(QString setting, QVariant value)
{
    if (!settings.contains(setting)) {
        return false;
    } else {
        if (value == settings[setting][st_setting])
            return true;
        else
            return false;
    }
}

void ConnectionSettings::alterSetting(QString setting, QVariant value)
{
    if (!settings.contains(setting) || value == settings[setting][st_setting]) {
        return;
    }
    PGresult *res = PQexec(conn, QString("SET %1 = %2").arg(setting).arg(value.toString()).toStdString().c_str());
    if (PQresultStatus(res) != PGRES_COMMAND_OK)
    {
        PQclear(res);
        throw SettingsException(QString("SET command failed: %1").arg(PQerrorMessage(conn)));
    }
    settings[setting][st_setting] = value;
}

void ConnectionSettings::alterMemSizeSetting(QString setting, int size, int unit)
{
    int mem_size;

    mem_size = static_cast<int>(floor( static_cast<double>(size) / 8 * pow(1024, unit)));

    alterSetting(setting, mem_size);
}

ConnectionSettings::mem_params ConnectionSettings::getMemParams(long size) const
{
    mem_params params;

    if (size < 128) {
        // kB
        params.size = size * 8;
        params.unit = MS_KB;
        params.min = 8;
        params.max = 1000;
        params.step = 8;
    } else if (size < 131072) {
        // MB
        params.size = size * 8 / 1024;
        params.unit = MS_MB;
        params.min = 0;
        params.max = 1000;
        params.step = 1;
    } else if (size < 134217728) {
        // GB
        params.size = static_cast<long>(size * 8 / pow(1024,2));
        params.unit = MS_GB;
        params.min = 0;
        params.max = 1000;
        params.step = 1;
    } else {
        // TB
        params.size = static_cast<long>(size * 8 / pow(1024,3));;
        params.unit = MS_TB;
        params.min = 0;
        params.max = 15;
        params.step = 1;
    }

    return params;
}

void ConnectionSettings::readSettings()
{
    char const *sql = "SELECT "
                      "  name, "
                      "  setting, "
                      "  unit, "
                      "  category, "
                      "  short_desc, "
                      "  extra_desc, "
                      "  context, "
                      "  vartype, "
                      "  source, "
                      "  min_val, "
                      "  max_val, "
                      "  enumvals, "
                      "  boot_val, "
                      "  reset_val, "
                      "  sourcefile, "
                      "  sourceline, "
                      "  pending_restart "
                      "FROM pg_catalog.pg_settings ";
    int tuples;
    int cols;
    QList<QVariant> row;

    PGresult *res = PQexec(conn, sql);
    tuples = PQntuples(res);
    cols = PQnfields(res);

    if (PQresultStatus(res) == PGRES_TUPLES_OK) {

        columns.clear();

        for (int i = 0; i < cols; i++) {
            columns.append(QString::fromStdString(PQfname(res, i)));
        }

        for (int i = 0; i < tuples; i++)
        {
            row.clear();
            for (int j = 0; j < cols; j++) {
                QVariant value = QString::fromStdString(PQgetvalue(res, i, j));
                row.append(value);
            }
            settings[row.at(st_name).toString()] = row;
        }
    }

    PQclear(res);
    return;
}

SettingsException *SettingsException::clone() const {
    return new SettingsException(*this);
}

void SettingsException::raise() const {
    throw *this;
}
