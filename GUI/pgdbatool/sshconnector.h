#ifndef SSHCONNECTOR_H
#define SSHCONNECTOR_H

#include <libssh2.h>
#include <QObject>
#include <QThread>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/time.h>

#include <fcntl.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/select.h>
#include <netdb.h>
#include <errno.h>

class SSHConnector: public QThread
{

public:

    enum
    {
        AUTH_NONE = 0,
        AUTH_PASSWORD,
        AUTH_KEYBOARD_INTERACTIVE,
        AUTH_PUBLICKEY = 4
    };

    SSHConnector(QObject *parent = nullptr) : QThread (parent)
    {

    }

    SSHConnector(const QString &tunnel_host, uint16_t ssh_port, const QString &local_listen_ip, uint16_t local_listen_port,
                 const QString &remote_dest_host, uint16_t remote_dest_port,
                 const QString &username, const QString &passwd,
                 const QString &privkey, const QString &pubkey, QObject *parent = nullptr);

    bool tunnelInitialize();

    QString getHostIP() const;

private:
    QString tunnel_host;

    uint16_t ssh_port;

    QString local_listen_ip;
    uint16_t local_listen_port;

    QString remote_dest_host;
    uint16_t remote_dest_port;

    QString username;
    QString password;
    QString private_key;
    QString public_key;

    QString server_ip;
    QString error;

    LIBSSH2_SESSION *session;
    LIBSSH2_CHANNEL *channel = nullptr;

    bool HostNameToIP(const QString &host, QStringList &ip_addresses);

};

#endif // SSHCONNECTOR_H
