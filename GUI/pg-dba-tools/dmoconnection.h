#ifndef DMOCONNECTION_H
#define DMOCONNECTION_H

#include <QObject>
#include <libpq-fe.h>

class DMOConnection : public QObject
{
    Q_OBJECT
public:
    explicit DMOConnection(QObject *parent = 0);
    ~DMOConnection();
    bool connect(void);
    QStringList getDatabases() const;

    void setHost(const QString &value);
    void setHostaddr(const QString &value);
    void setPort(int value);
    void setDatabase(const QString &value);
    void setUser(const QString &value);
    void setPassword(const QString &value);
    void setService(const QString &value);
    void setRealm(const QString &value);
    void setOptions(const QString &value);
    void setSslmode(const QString &value);
    void setRequiressl(const QString &value);
    void setSslcert(const QString &value);
    void setSslkey(const QString &value);
    void setSslrootcert(const QString &value);
    void setSslcrl(const QString &value);
    void setKrbsrvname(const QString &value);
    void setConnect_timeout(int value);
    void setGsslib(const QString &value);

signals:

public slots:

private:
    QString host;
    QString hostaddr;
    int   port;
    QString database;
    QString user;
    QString password;
    QString passfile;
    QString service;
    QString realm;
    QString options;
    QString sslmode;
    QString requiressl;
    QString sslcert;
    QString sslkey;
    QString sslrootcert;
    QString sslcrl;
    QString krbsrvname;
    QString gsslib;
    int   connect_timeout;

    PGconn *conn;
    int   status;

    QStringList databases;
};

#endif // DMOCONNECTION_H
