#ifndef SOCKDGRAM_H
#define SOCKDGRAM_H

#include <netinet/in.h>

namespace posix{

class SockDgram {
public:
    enum role_t { CLIENT = 0, SERVER } ;
    enum block_t { BLOCK = 0, NONBLOCK } ;

private:
    int sockd;
    struct sockaddr_in UDPAddr;

    int make_socket_non_blocking(int sfd);
public:
    SockDgram(void);
    ~SockDgram(void);

    bool Open(const role_t role, const char *ip, const uint32_t port, const block_t blockValue);
    bool Close(void);
    bool Read(void *buffer, size_t size, ssize_t *recvLen);
    bool Write(const void *buffer, size_t size, ssize_t *sendLen);
};

}

#endif /* SOCKDGRAM_H */

