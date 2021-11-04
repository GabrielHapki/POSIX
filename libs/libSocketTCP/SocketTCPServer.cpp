#include "SocketTCPServer.hpp"
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#include <iostream>
#include <cerrno>
#include <cstring>
#include <clocale>

Server::Server(void) {
    this->sockd = -1;
    this->sockdl = -1;
}

Server::~Server(void) {
    Close();
}

bool Server::Open(const unsigned int port, const uint8_t numListen) {
    if (this->sockd > 0) {
        this->Close();
        return false;
    }

    if ((this->sockd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
        std::cerr << "Error(" << errno << "): " << std::strerror(errno) << std::endl;
        return false;
    }

    int on;
    struct sockaddr_in my_name;
    setsockopt(sockd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof (on));
    bzero((char *) &my_name, sizeof (my_name));
    my_name.sin_family = AF_INET;
    my_name.sin_addr.s_addr = INADDR_ANY;
    my_name.sin_port = htons(port);

    if (bind(this->sockd, (struct sockaddr*) &my_name, sizeof (my_name)) < 0) {
        std::cerr << "Error(" << errno << "): " << std::strerror(errno) << std::endl;
        return false;
    }

    if (listen(this->sockd, numListen) < 0) {
        std::cerr << "Error(" << errno << "): " << std::strerror(errno) << std::endl;
        return false;
    }

    return true;
}

bool Server::Close(void) {
    if (this->sockd < 0) {
        return false;
    }

    if (CloseAccept()) {
        if (close(sockd) == 0) {
            sockd = -1;
            return true;
        }
        std::cerr << "Error(" << errno << "): " << std::strerror(errno) << std::endl;
    }
    return false;
}

bool Server::Accept(void) {
    if (this->sockd < 0) {
        return false;
    }

    unsigned int addrlen;
    struct sockaddr_in peer_name;
    addrlen = sizeof (peer_name);
    this->sockdl = accept(sockd, (struct sockaddr *) &peer_name, (socklen_t *) & addrlen);
    if (this->sockdl < 0) {
        std::cerr << "Error(" << errno << "): " << std::strerror(errno) << std::endl;
        return false;
    }

    return true;
}

bool Server::CloseAccept(void)
{
    if (this->sockdl < 0) {
        return false;
    }

    if (close(this->sockdl) == 0) {
        sockdl = -1;
        return true;
    }
    std::cerr << "Error(" << errno << "): " << std::strerror(errno) << std::endl;
    return false;
}

Server::status_t Server::Write(const void * buffer, size_t size, ssize_t *sendLen) {
    if (this->sockdl < 0)
        return CLOSE;

    if ((buffer == nullptr) || sendLen == nullptr)
        return ERROR;

    *sendLen = send(this->sockdl, buffer, size, MSG_NOSIGNAL);
    if (*sendLen > 0) {
        return OK;
    }
    std::cerr << "Error(" << errno << "): " << std::strerror(errno) << std::endl;
    if (this->CloseAccept())
        return CLOSE;

    return ERROR;
}

Server::status_t Server::Read(void * buffer, size_t size, ssize_t *recvLen) {
    if (this->sockdl < 0)
        return CLOSE;

    if ((buffer == nullptr) || recvLen == nullptr)
        return ERROR;

    *recvLen = recv(this->sockdl, buffer, size, MSG_DONTWAIT);
    if (*recvLen < 0) {
        return NODATA;
    } else
    if (*recvLen > 0) {
        return OK;
    } else
    if (*recvLen == 0) {
        std::cerr << "Error(" << errno << "): " << std::strerror(errno) << std::endl;
        if (this->CloseAccept())
            return CLOSE;
    }

    return ERROR;
}
