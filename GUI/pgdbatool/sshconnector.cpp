#include "sshconnector.h"

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

#ifndef INADDR_NONE
#define INADDR_NONE (in_addr_t)-1
#endif

SSHConnector::SSHConnector(char *host, char *username, char *passwd,
                           char *privkey, char *pubkey) :
    server_ip(host),
    username(username),
    password(passwd),
    private_key(privkey),
    public_key(pubkey)

{

    listener = nullptr;
    session = nullptr;
    channel = nullptr;


}

int SSHConnector::forwardTunnel(int remote_port, int local_port)
{

}


