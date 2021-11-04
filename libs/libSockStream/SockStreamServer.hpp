#ifndef SOCKSTREAMSERVER_H
#define SOCKSTREAMSERVER_H

#include <stdint.h>
#include <cstddef>
#include <sys/types.h>

class SockStreamServer {
public:
    enum status_t { ERROR, CLOSE, OK, NODATA };
private:
    int sockd;
    int sockdl;

public:
    SockStreamServer(void);
    virtual ~SockStreamServer(void);

    bool Open(const unsigned int port, const uint8_t numListen);
    bool Close(void);
    bool Accept(void);
    bool CloseAccept(void);
    status_t Read(void * buffer, size_t size, ssize_t *sendLen);
    status_t Write(const void * buffer, size_t size, ssize_t *recvLen);
};

#endif /* SOCKSTREAMSERVER_H */
