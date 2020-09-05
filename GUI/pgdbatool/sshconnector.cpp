#include "sshconnector.h"

#ifndef INADDR_NONE
#define INADDR_NONE (in_addr_t)-1
#endif

#ifdef USE_SSH_TUNNELS

SSHConnector::SSHConnector(const QString &tunnel_host, uint16_t ssh_port, const QString &local_listen_ip, uint16_t local_listen_port,
                           const QString &remote_dest_host, uint16_t remote_dest_port,
                           const QString &username, const QString &passwd,
                           const QString &privkey, const QString &pubkey, QObject *parent) :
    QThread(parent),
    tunnel_host(tunnel_host),
    ssh_port(ssh_port),
    local_listen_ip(local_listen_ip),
    local_listen_port(local_listen_port),
    remote_dest_host(remote_dest_host),
    remote_dest_port(remote_dest_port),
    username(username),
    password(passwd),
    private_key(privkey),
    public_key(pubkey)
{

    error = "";

}

bool SSHConnector::tunnelInitialize()
{
    QStringList ips;
    int rc, i, auth = AUTH_NONE;
    struct sockaddr_in sin;
    socklen_t sinlen;
    const char *fprint;
    char *userauthlist;
    int sockopt, sock = -1;
    int listensock = -1 /*, forwardsock = -1*/;

    QString msg;

    if (!HostNameToIP(tunnel_host, ips)) {
        return false;
    }

    server_ip = ips[0];

    rc = libssh2_init (0);

    if (rc != 0)
    {
        error = QString("libssh2 initialization failed with error code %1.").arg(rc);
        return false;
    }

    sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

    if(sock == -1) {
        error = "Socket error";
        return false;
    }

    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = inet_addr(server_ip.toStdString().c_str());
    if(INADDR_NONE == sin.sin_addr.s_addr) {
        error = "Error in inet address.";
        return false;
    }

    sin.sin_port = htons(ssh_port);
    if(::connect(sock, reinterpret_cast<struct sockaddr*>(&sin),
               sizeof(struct sockaddr_in)) != 0) {
        error = QString("Failed to connect to host %1.").arg(tunnel_host);
        return false;
    }

    session = libssh2_session_init();
    if (!session) {
        error = "Could not initialize SSH session.";
        return false;
    }

    rc = libssh2_session_handshake(session, sock);

    if(rc) {
        error = QString("Error when starting up SSH session: %1.").arg(rc);
        return false;
    }

    fprint = libssh2_hostkey_hash(session, LIBSSH2_HOSTKEY_HASH_SHA1);

    for(i = 0; i < 20; i++)
        fingerprint +=  QString().sprintf("%02X ", static_cast<unsigned char>(fprint[i]));

    userauthlist = libssh2_userauth_list(session, username.toStdString().c_str(), strlen(username.toStdString().c_str()));

    // Ignore Keyboard interactive method

    if(strstr(userauthlist, "password"))
        auth |= AUTH_PASSWORD;

    if(strstr(userauthlist, "publickey"))
        auth |= AUTH_PUBLICKEY;

    if((auth & AUTH_PUBLICKEY) && !public_key.isEmpty())
                auth = AUTH_PUBLICKEY;
    else if ((auth & AUTH_PASSWORD ) && !password.isEmpty())
        auth = AUTH_PASSWORD;

    if(auth & AUTH_PASSWORD) {

        if(libssh2_userauth_password(session, username.toStdString().c_str(), password.toStdString().c_str())) {
            error = "Authentication by password failed.";
            return false;
        }

    } else if(auth & AUTH_PUBLICKEY) {

        if(libssh2_userauth_publickey_fromfile(session, username.toStdString().c_str(),
                                               public_key.toStdString().c_str(), private_key.toStdString().c_str(),
                                               password.toStdString().c_str())) {
            error = "Authentication by public key failed.";
            return false;
        }

    } else {

        error = "No supported authentication methods found.";
        return false;

    }

    listensock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

    if(listensock == -1) {
        error = "Listen Socket error";
        return false;
    }

    sin.sin_family = AF_INET;
    sin.sin_port = htons(local_listen_port);
    sin.sin_addr.s_addr = inet_addr(local_listen_ip.toStdString().c_str());

    if(INADDR_NONE == sin.sin_addr.s_addr) {
        error = "inet_addr";
        return false;
    }

    sockopt = 1;

    setsockopt(listensock, SOL_SOCKET, SO_REUSEADDR, &sockopt,
               sizeof(sockopt));
    sinlen = sizeof(sin);

    if(-1 == bind(listensock, (struct sockaddr *)&sin, sinlen)) {
        error = "bind";
        return false;
    }

    if(-1 == listen(listensock, 2)) {
        error = "listen";
        return false;
    }

    msg = QString("Waiting for TCP connection on %1, %2").arg(inet_ntoa(sin.sin_addr), ntohs(sin.sin_port));

    return true;
}

QString SSHConnector::getHostIP() const
{
    return server_ip;
}

QString SSHConnector::getFingerprint() const
{
    return fingerprint;
}


bool SSHConnector::HostNameToIP(const QString &host, QStringList &ip_addresses)
{
    typedef const char *(*inet_ntop_t) (int af, const void *src, char *dst, socklen_t size);
    static inet_ntop_t gs_fnPtr_inet_ntop = &inet_ntop;
    struct addrinfo hints, *res, *p;
    int status;
    char ipstr[INET6_ADDRSTRLEN];

    memset(&hints, 0, sizeof hints);
    memset(&ipstr, 0, sizeof ipstr);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_ADDRCONFIG | AI_PASSIVE;

    if ((status = getaddrinfo(host.toStdString().c_str(), nullptr, &hints, &res)) != 0)
    {
        error = QString("getaddrinfo failed with error code: %1").arg(status);
        return false;
    }

    for(p = res; p != nullptr; p = p->ai_next)
    {
        void *addr;
        if (p->ai_family == AF_INET)
        {
            struct sockaddr_in *ipv4 = reinterpret_cast<struct sockaddr_in *>(p->ai_addr);
            addr = &(ipv4->sin_addr);

            if(nullptr != gs_fnPtr_inet_ntop(p->ai_family, addr, ipstr, sizeof ipstr))
                ip_addresses << QString(ipstr);
        }
    }

    if(res)
    {
        freeaddrinfo(res);
    }

    if(!ip_addresses.isEmpty())
    {
        return true;
    }

    return false;

}

#endif




