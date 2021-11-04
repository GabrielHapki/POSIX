#ifndef SOCKETTCPSERVER_H
#define SOCKETTCPSERVER_H

#include <stdint.h>
#include <cstddef>
#include <sys/types.h>

class Server {
public:
    enum status_t { ERROR, CLOSE, OK, NODATA };
private:
    int sockd;
    int sockdl;

public:
    Server(void);
    virtual ~Server(void);

    bool Open(const unsigned int port, const uint8_t numListen);
    bool Close(void);
    bool Accept(void);
    bool CloseAccept(void);
    status_t Read(void * buffer, size_t size, ssize_t *sendLen);
    status_t Write(const void * buffer, size_t size, ssize_t *recvLen);
};

#endif /* SOCKETTCPSERVER_H */
