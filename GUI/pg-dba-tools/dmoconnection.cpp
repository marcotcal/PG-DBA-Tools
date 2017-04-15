#include "dmoconnection.h"
#include <QMessageBox>

DMOConnection::DMOConnection(QObject *parent) : QObject(parent)
{
    host = QString();
    hostaddr = QString();
    port = 5432;
    database = "postgres";
    user = QString();
    password = QString();
    passfile = QString();
    service = QString();
    realm = QString();
    options = QString();
    sslmode = QString();
    requiressl = QString();
    sslcert = QString();
    sslkey = QString();
    sslrootcert = QString();
    sslcrl = QString();
    krbsrvname = QString();
    gsslib = QString();
    connect_timeout = -1;

    conn = NULL;
    status = CONNECTION_BAD;

    if (qEnvironmentVariableIsSet("PGHOST"))
        options = qgetenv("PGHOST");

    if (qEnvironmentVariableIsSet("PGHOSTADDR"))
        hostaddr = qgetenv("PGHOSTADDR");

    if (qEnvironmentVariableIsSet(" PGPORT"))
        hostaddr = qgetenv(" PGPORT");

    if (qEnvironmentVariableIsSet("PGDATABASE"))
        password = qgetenv("PGDATABASE");

    if (qEnvironmentVariableIsSet("PGUSER"))
        user = qgetenv("PGUSER");

    if (qEnvironmentVariableIsSet("PGPASSWORD"))
        password = qgetenv("PGPASSWORD");

    if (qEnvironmentVariableIsSet("PGPASSFILE"))
        password = qgetenv("PGPASSFILE");

    if (qEnvironmentVariableIsSet("PGSERVICE"))
        password = qgetenv("PGSERVICE");

    if (qEnvironmentVariableIsSet("PGREALM"))
        password = qgetenv("PGREALM");

    if (qEnvironmentVariableIsSet("PGOPTIONS"))
        options = qgetenv("PGOPTIONS");

    if (qEnvironmentVariableIsSet("PGSSLMODE"))
        password = qgetenv("PGSSLMODE");

    if (qEnvironmentVariableIsSet("PGREQUIRESSL"))
        password = qgetenv("PGREQUIRESSL");

    if (qEnvironmentVariableIsSet("PGSSLCERT"))
        password = qgetenv("PGSSLCERT");

    if (qEnvironmentVariableIsSet("PGSSLKEY"))
        password = qgetenv("PGSSLKEY");

    if (qEnvironmentVariableIsSet("PGSSLROOTCERT"))
        password = qgetenv("PGSSLROOTCERT");

    if (qEnvironmentVariableIsSet("PGSSLCRL"))
        password = qgetenv("PGSSLCRL");

    if (qEnvironmentVariableIsSet("PGKRBSRVNAME"))
        password = qgetenv("PGKRBSRVNAME");

    if (qEnvironmentVariableIsSet("PGGSSLIB"))
        password = qgetenv("PGGSSLIB");

    if (qEnvironmentVariableIsSet("PGCONNECT_TIMEOUT"))
        password = qgetenv("PGCONNECT_TIMEOUT");
}

DMOConnection::~DMOConnection()
{
    if (conn)
        PQfinish(conn);
}

bool DMOConnection::connect()
{
    PGresult   *res;
    int nFields;
    QStringList params;
    QString connstr;

    if(!host.isNull())
        params << QString("host=%1").arg(host);

    if(!hostaddr.isNull())
        params << QString("hostaddr=%1").arg(hostaddr);

    params << QString("port=%1").arg(port);

    if(!database.isNull())
        params << QString("dbname=%1").arg(database);

    if(!user.isNull())
        params << QString("user=%1").arg(user);

    if(!password.isNull())
        params << QString("password=%1").arg(password);

    if(!service.isNull())
        params << QString("service=%1").arg(service);

    connstr = params.join(" ");
    conn = PQconnectdb(connstr.toUtf8().data());

    status = PQstatus(conn);

    if (status == CONNECTION_OK) {

        res = PQexec(conn,"BEGIN");
        if (PQresultStatus(res) != PGRES_COMMAND_OK) {
            QMessageBox::critical(NULL, tr("Error"),QString(tr("BEGIN command failed: %1")).arg(PQerrorMessage(conn)));
            PQclear(res);
            return false;
        }
        PQclear(res);
        res = PQexec(conn, "DECLARE dbn CURSOR FOR select datname from pg_database");
        if (PQresultStatus(res) != PGRES_COMMAND_OK)
        {
            QMessageBox::critical(NULL, tr("Error"),QString(tr("DECLARE CURSOR failed: %1")).arg(PQerrorMessage(conn)));
            PQclear(res);
            return false;
        }
        PQclear(res);

        res = PQexec(conn, "FETCH ALL in dbn");
        if (PQresultStatus(res) != PGRES_TUPLES_OK)
        {
            QMessageBox::critical(NULL, tr("Error"),QString(tr("FECHALL failed: %1")).arg(PQerrorMessage(conn)));
            PQclear(res);
            return false;
        }
        nFields = PQnfields(res);
        //for (int i = 0; i < nFields; i++)
        //    qDebug() << PQfname(res, i);
        for (int i = 0; i < PQntuples(res); i++) {
            for (int j = 0; j < nFields; j++)
                databases << PQgetvalue(res, i, j);
        }

        PQclear(res);

        res = PQexec(conn, "CLOSE dbn");
        PQclear(res);

        res = PQexec(conn, "END");
        PQclear(res);

    } else {
        QMessageBox::critical(NULL, tr("Error"), QString(tr("Error opening database %1")).arg( PQerrorMessage(conn)));
        return false;
    }

    return true;
}

QStringList DMOConnection::getDatabases() const
{
    return databases;
}

void DMOConnection::setHost(const QString &value)
{
    host = value;
}

void DMOConnection::setHostaddr(const QString &value)
{
    hostaddr = value;
}

void DMOConnection::setPort(int value)
{
    port = value;
}

void DMOConnection::setDatabase(const QString &value)
{
    database = value;
}

void DMOConnection::setUser(const QString &value)
{
    user = value;
}

void DMOConnection::setPassword(const QString &value)
{
    password = value;
}

void DMOConnection::setOptions(const QString &value)
{
    options = value;
}

void DMOConnection::setSslmode(const QString &value)
{
    sslmode = value;
}

void DMOConnection::setRequiressl(const QString &value)
{
    requiressl = value;
}

void DMOConnection::setSslcert(const QString &value)
{
    sslcert = value;
}

void DMOConnection::setSslkey(const QString &value)
{
    sslkey = value;
}

void DMOConnection::setSslrootcert(const QString &value)
{
    sslrootcert = value;
}

void DMOConnection::setSslcrl(const QString &value)
{
    sslcrl = value;
}

void DMOConnection::setKrbsrvname(const QString &value)
{
    krbsrvname = value;
}

void DMOConnection::setConnect_timeout(int value)
{
    connect_timeout = value;
}

void DMOConnection::setGsslib(const QString &value)
{
    gsslib = value;
}

void DMOConnection::setService(const QString &value)
{
    service = value;
}

void DMOConnection::setRealm(const QString &value)
{
    realm = value;
}

