#ifndef SOCKSTREAMCLIENT_H
#define SOCKSTREAMCLIENT_H

#include <stdint.h>
#include <stddef.h>
#include <sys/types.h>

class SockStreamClient {
public:
    enum status_t { ERROR, CLOSE, OK, NODATA };
private:
    int sockd;
public:
    SockStreamClient(void);
    virtual ~SockStreamClient(void);

    bool Open(const char * ip_server, const int Port);
    bool Close(void);
    status_t Write(const void * buffer, size_t size, ssize_t *sendLen);
    status_t Read(void * buffer, size_t size, ssize_t *recvLen);
};

#endif /* SOCKSTREAMCLIENT_H */
