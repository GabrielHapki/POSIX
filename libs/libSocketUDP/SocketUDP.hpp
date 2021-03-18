#ifndef SOCKETUDP_H
#define SOCKETUDP_H

#include <stdint.h>
#include <netinet/in.h>

typedef enum SERVER_CLIENT {
    CLIENT = 0,
    SERVER = 1
} udpserver_t;

class UDP {
private:
    int SockFD;
    struct sockaddr_in UDPAddr;
public:
    UDP(void);
    ~UDP(void);
    bool Open(udpserver_t BoolServer, const char *IP, const uint32_t Port, const bool NonBlocking);
    bool Close(void);
    bool Read(void * Buffer, size_t Size, int32_t *RecvLen);
    bool Write(const void * Buffer, size_t Size, int32_t *SendLen);
};

#endif /* SOCKETUDP_H */

