#ifndef SOCKETTCPCLIENT_H
#define SOCKETTCPCLIENT_H

#include "SocketTCPCommon.hpp"
#include <stdint.h>
#include <stddef.h>

namespace SocketTCP {

    class Client {
    private:
        int sockd;
    public:
        Client(void);
        ~Client(void);

        STATUS Open(const char * ip_server, const int Port);
        STATUS Write(const void * buffer, size_t Size, int32_t *SendLen);
        STATUS Read(void * buffer, size_t Size, int32_t *RecvLen);
        STATUS Close(void);
    };

}

#endif /* SOCKETTCPCLIENT_H */
