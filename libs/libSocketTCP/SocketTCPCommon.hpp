#ifndef SOCKETTCPCOMMON_H
#define SOCKETTCPCOMMON_H

namespace SocketTCP {

    typedef enum {
        LISTEN_FAIL = -5,
        BIND_FAIL = -4,
        CONNECTION_FAIL = -3,
        SOCKET_CREATION_FAIL = -2,
        CLOSE = -1,
        OPEN,
    } STATUS;

}

#endif /* SOCKETTCPCOMMON_H */

