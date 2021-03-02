#ifndef SOCKETTCPSERVER_H
#define SOCKETTCPSERVER_H

#include "SocketTCPCommon.hpp"
#include <stdint.h>
#include <cstddef>

namespace SocketTCP {

    class Server {
    private:
        int sockd;
        int sockdl;

        int setNonblocking(unsigned int fd);
    public:
        Server();
        virtual ~Server();

        STATUS Open(unsigned int Port);
        STATUS Accept(void);
        STATUS Read(void * buffer, size_t Size, int32_t *SendLen);
        STATUS Write(void * buffer, size_t Size, int32_t *RecvLen);
        STATUS Close();
    };

}

#endif /* SOCKETTCPSERVER_H */
