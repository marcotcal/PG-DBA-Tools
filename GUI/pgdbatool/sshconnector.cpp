#include "sshconnector.h"

#ifndef INADDR_NONE
#define INADDR_NONE (in_addr_t)-1
#endif

SSHConnector::SSHConnector(const QString &tunnel_host, const QString &remode_dest_host, uint16_t remote_dest_port,
                           const QString &username, const QString &passwd, const QString &pubkey,
                           const QString &privkey, uint16_t tunnel_port, QObject *parent) :
    QThread(parent),
    tunnel_host(tunnel_host),
    remote_dest_host(remode_dest_host),
    remote_dest_port(remote_dest_port),
    username(username),
    password(passwd),
    private_key(privkey),
    public_key(pubkey),
    tunnel_port(tunnel_port)
{

    error_log.clear();

}

bool SSHConnector::tunnelInitialize()
{
    QStringList ips;
    int rc, auth = AUTH_NONE;
    const char *fingerprint;

    if (!ResolveDNS(tunnel_host, ips)) {
        return false;
    }

    host_ip = ips[0];

    rc = libssh2_init (0);

    if (rc != 0)
    {
        error_log.append(QString("libssh2 initialization failed with error code %1").arg(rc));
        return false;
    }

    m_sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    m_sin.sin_family = AF_INET;
    if (INADDR_NONE == (m_sin.sin_addr.s_addr = inet_addr(host_ip.toStdString().c_str())))
    {
        // TODO - Replace inet_addr to get a more clear error code
        error_log.append(QString("SSH error: Error in inet address"));
        return false;
    }

    m_sin.sin_port = htons(tunnel_port);

    /*

    if (connect(m_sock, (struct sockaddr *)(&m_sin),
                sizeof(struct sockaddr_in)) != 0)
    {
        error_log.append("SSH error: Could not connect to socket");
        return false;
    }

    session = libssh2_session_init();

    if (!session)
    {
        error_log.append("SSH error: Could not initialize SSH session!");
        return false;
    }

    rc = libssh2_session_handshake(session, m_sock);
    if (rc)
    {
        error_log.append("SSH error: Error when starting up SSH session");
        return false;
    }

    fingerprint = libssh2_hostkey_hash(session, LIBSSH2_HOSTKEY_HASH_SHA1);

    */

    // TODO - Continue from here

    return true;
}

QString SSHConnector::getHostIP() const
{
    return host_ip;
}


bool SSHConnector::ResolveDNS(const QString &host, QStringList &ip_addresses)
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
        error_log.append(QString("getaddrinfo failed with error code: %1").arg(status));
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





