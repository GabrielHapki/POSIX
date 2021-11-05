#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include "SockDgram.hpp"

#include <iostream>
#include <cerrno>
#include <cstring>
#include <clocale>

UDP::UDP(void) : sockd(-1)
{

}

UDP::~UDP()
{

}

int UDP::make_socket_non_blocking(int sfd) {
    int flags, s;
    flags = fcntl(sfd, F_GETFL, 0);
    if (flags == -1) {
        perror("fcntl");
        return -1;
    }
    flags |= O_NONBLOCK;
    s = fcntl(sfd, F_SETFL, flags);
    if (s == -1) {
        perror("fcntl");
        return -1;
    }
    return 0;
}

bool UDP::Open(const role_t role, const char *ip, const uint32_t port, const block_t blockValue) {

    this->sockd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (this->sockd < 0) {
        std::cerr << "Error(" << errno << "): " << std::strerror(errno) << std::endl;
        return false;
    }

    if (blockValue == NONBLOCK) {
        make_socket_non_blocking(this->sockd);
    }

    bzero((char *) & this->UDPAddr, sizeof(this->UDPAddr));
    this->UDPAddr.sin_family = AF_INET;
    this->UDPAddr.sin_port = htons((unsigned short) port);
    if (role == CLIENT) {
        this->UDPAddr.sin_addr.s_addr = inet_addr(ip);
    }
    memset(this->UDPAddr.sin_zero, '\0', sizeof (this->UDPAddr.sin_zero));

    if (role == SERVER) {
        if (bind(this->sockd, (struct sockaddr *) & this->UDPAddr, sizeof (this->UDPAddr)) < 0) {
            std::cerr << "Error(" << errno << "): " << std::strerror(errno) << std::endl;
            return false;
        }
    }

    return true;
}

bool UDP::Close(void) {
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

bool UDP::Read(void * buffer, size_t size, ssize_t *recvLen) {
    if (this->sockd < 0)
        return false;

    if ((buffer == nullptr) || recvLen == nullptr)
        return false;

    int len = sizeof(this->UDPAddr);
    *recvLen = recvfrom(this->sockd, buffer, size, 0, (struct sockaddr *)&this->UDPAddr, (socklen_t*)&len);
    if (*recvLen > 0) {
        return true;
    }
    std::cerr << "Error(" << errno << "): " << std::strerror(errno) << std::endl;
    return false;
}

bool UDP::Write(const void * buffer, size_t size, ssize_t *sendLen) {
    if (this->sockd < 0)
        return false;

    if ((buffer == nullptr) || sendLen == nullptr)
        return false;

    *sendLen = sendto(this->sockd, buffer, size, 0, (struct sockaddr *)&this->UDPAddr, sizeof(this->UDPAddr));
    if (*sendLen > 0)
        return true;
    std::cerr << "Error(" << errno << "): " << std::strerror(errno) << std::endl;
    return false;
}
