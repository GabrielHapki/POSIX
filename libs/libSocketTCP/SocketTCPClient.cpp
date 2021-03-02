#include "SocketTCPClient.hpp"
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

namespace SocketTCP {

    Client::Client(void) {
        this->sockd = -1;
    }

    Client::~Client(void) {
        Close();
    }

    STATUS Client::Open(const char * ip_server, const int Port) {
        if (this->sockd > 0) {
            this->Close();
            return CLOSE;
        }

        this->sockd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        if (this->sockd == -1) {
            return SOCKET_CREATION_FAIL;
        }

        struct sockaddr_in serv_name;
        serv_name.sin_family = AF_INET;
        serv_name.sin_addr.s_addr = inet_addr(ip_server);
        serv_name.sin_port = htons(Port);

        if (connect(this->sockd, (struct sockaddr*) &serv_name, sizeof (serv_name)) == -1) {
            return CONNECTION_FAIL;
        }

        return OPEN;
    }

    STATUS Client::Write(void * buffer, size_t Size, int32_t *SendLen) {
        size_t Len = 0;
        if (this->sockd > 0) {
            Len = write(this->sockd, buffer, Size);
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

    STATUS Client::Read(void * buffer, size_t Size, int32_t *RecvLen) {
        size_t Len = 0;
        if (this->sockd > 0) {
            Len = read(this->sockd, buffer, Size);
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

    STATUS Client::Close(void) {
        close(this->sockd);
        this->sockd = -1;
        return CLOSE;
    }

}