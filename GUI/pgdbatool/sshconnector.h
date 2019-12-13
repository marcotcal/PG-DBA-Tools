#ifndef SSHCONNECTOR_H
#define SSHCONNECTOR_H

#include <libssh2.h>

class SSHConnector
{

public:

    enum {
        AUTH_NONE = 0,
        AUTH_PASSWORD,
        AUTH_PUBLICKEY
    };

    SSHConnector(char *host, char *username, char *passwd,
                 char *privkey, char *pubkey);

    int forwardTunnel(int remote_port, int local_port);

private:
    char *server_ip;
    char *username;
    char *password;
    char *private_key;
    char *public_key;

    LIBSSH2_LISTENER *listener;
    LIBSSH2_SESSION *session;
    LIBSSH2_CHANNEL *channel;
};

#endif // SSHCONNECTOR_H
