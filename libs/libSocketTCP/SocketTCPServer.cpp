#include "SocketTCPServer.hpp"
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

namespace SocketTCP {

    Server::Server(void) {
        this->sockd = -1;
        this->sockdl = -1;
    }

    Server::~Server(void) {
        Close();
    }

    STATUS Server::Open(unsigned int Port) {
        if (this->sockd > 0) {
            this->Close();
            return CLOSE;
        }

        if ((this->sockd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
            return SOCKET_CREATION_FAIL;
        }

        int on;
        struct sockaddr_in my_name;
        setsockopt(sockd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof (on));
        bzero((char *) &my_name, sizeof (my_name));
        my_name.sin_family = AF_INET;
        my_name.sin_addr.s_addr = INADDR_ANY;
        my_name.sin_port = htons(Port);

        if (bind(this->sockd, (struct sockaddr*) &my_name, sizeof (my_name)) < 0) {
            return BIND_FAIL;
        }

        if (listen(this->sockd, 1) < 0) {
            return LISTEN_FAIL;
        }

        return OPEN;
    }

    STATUS Server::Accept(void) {
        if (this->sockd < 0) {
            return CLOSE;
        }

        unsigned int addrlen;
        struct sockaddr_in peer_name;
        addrlen = sizeof (peer_name);
        this->sockdl = accept(sockd, (struct sockaddr *) &peer_name, (socklen_t *) & addrlen);
        if (this->sockdl < 0) {
            return CLOSE;
        }
        return OPEN;
    }

    STATUS Server::Write(void * buffer, size_t Size, int32_t *SendLen) {
        ssize_t Len = 0;
        if (this->sockdl > 0) {
            Len = write(this->sockdl, buffer, Size);
            *SendLen = Len;
            if (Len <= 0) {
                this->Close();
                return CLOSE;
            }
        } else {
            return CLOSE;
        }
        return OPEN;
    }

    STATUS Server::Read(void * buffer, size_t Size, int32_t *RecvLen) {
        ssize_t Len = 0;
        if (this->sockdl > 0) {
            Len = read(this->sockdl, buffer, Size);
            *RecvLen = Len;
            if (Len <= 0) {
                this->Close();
                return CLOSE;
            }
        } else {
            return CLOSE;
        }
        return OPEN;
    }

    STATUS Server::Close() {
        close(this->sockdl);
        close(this->sockd);
        sockd = -1;
        sockdl = -1;
        return CLOSE;
    }

}