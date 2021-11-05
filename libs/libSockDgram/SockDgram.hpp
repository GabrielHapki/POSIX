#ifndef SOCKETUDP_H
#define SOCKETUDP_H

#include <stdint.h>
#include <netinet/in.h>

class UDP {
public:
    enum role_t { CLIENT, SERVER } ;
    enum block_t { BLOCK, NONBLOCK } ;

private:
    int sockd;
    struct sockaddr_in UDPAddr;
    int make_socket_non_blocking(int sfd);

public:
    UDP(void);
    virtual ~UDP(void);
    bool Open(const role_t role, const char *ip, const uint32_t port, const block_t blockValue);
    bool Close(void);
    bool Read(void *buffer, size_t size, ssize_t *recvLen);
    bool Write(const void *buffer, size_t size, ssize_t *sendLen);
};

#endif /* SOCKETUDP_H */

