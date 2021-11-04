#include "SockStreamClient.hpp"
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#include <iostream>
#include <cerrno>
#include <cstring>
#include <clocale>

SockStreamClient::SockStreamClient(void) {
    this->sockd = -1;
}

SockStreamClient::~SockStreamClient(void) {
    Close();
}

bool SockStreamClient::Open(const char * ip_server, const int Port) {
    if (this->sockd > 0) {
        this->Close();
        return false;
    }

    this->sockd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (this->sockd == -1) {
        std::cerr << "Error(" << errno << "): " << std::strerror(errno) << std::endl;
        return false;
    }

    struct sockaddr_in serv_name;
    serv_name.sin_family = AF_INET;
    serv_name.sin_addr.s_addr = inet_addr(ip_server);
    serv_name.sin_port = htons(Port);

    if (connect(this->sockd, (struct sockaddr*) &serv_name, sizeof (serv_name)) == -1) {
        std::cerr << "Error(" << errno << "): " << std::strerror(errno) << std::endl;
        return false;
    }

    return true;
}

bool SockStreamClient::Close(void) {
    if (this->sockd < 0) {
        return false;
    }

    if (close(this->sockd) == 0) {
        this->sockd = -1;
        return true;
    }
    std::cerr << "Error(" << errno << "): " << std::strerror(errno) << std::endl;
    return false;
}

SockStreamClient::status_t SockStreamClient::Write(const void * buffer, size_t Size, ssize_t *SendLen) {
    if (this->sockd < 0)
        return CLOSE;

    if ((buffer == nullptr) || SendLen == nullptr)
        return ERROR;

    *SendLen = send(this->sockd, buffer, Size, MSG_NOSIGNAL);
    if (*SendLen > 0) {
        return OK;
    }
    std::cerr << "Error(" << errno << "): " << std::strerror(errno) << std::endl;
    if (this->Close())
        return CLOSE;

    return ERROR;
}

SockStreamClient::status_t SockStreamClient::Read(void * buffer, size_t Size, ssize_t *RecvLen) {
    if (this->sockd < 0)
        return CLOSE;

    if ((buffer == nullptr) || RecvLen == nullptr)
        return ERROR;

    *RecvLen = recv(this->sockd, buffer, Size, MSG_DONTWAIT);
    if (*RecvLen < 0) {
        return NODATA;
    } else
    if (*RecvLen > 0) {
        return OK;
    } else
    if (*RecvLen == 0) {
        std::cerr << "Error(" << errno << "): " << std::strerror(errno) << std::endl;
        if (this->Close())
            return CLOSE;
    }

    return ERROR;
}

