#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <errno.h>
#include "SocketUDP.hpp"

UDP::UDP(void) {
}

UDP::~UDP() {
}

static int make_socket_non_blocking(int sfd) {
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

bool UDP::Open(udpserver_t BoolServer, const char *IP, const uint32_t Port, const bool NonBlocking) {

    this->SockFD = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (this->SockFD < 0) {
        fprintf(stderr, "socket errno: %d - %s\n", errno, strerror(errno));
        return false;
    }
    if (NonBlocking == true) {
        make_socket_non_blocking(this->SockFD);
    }
    uint16_t on = 1;
    setsockopt(this->SockFD, SOL_SOCKET, SO_REUSEADDR, (const void *) &on, sizeof (on));
    bzero((char *) & this->UDPAddr, sizeof (this->UDPAddr));
    this->UDPAddr.sin_family = AF_INET;
    this->UDPAddr.sin_port = htons((unsigned short) Port);
    if (BoolServer == CLIENT) {
        this->UDPAddr.sin_addr.s_addr = inet_addr(IP);
    }
    memset(this->UDPAddr.sin_zero, '\0', sizeof (this->UDPAddr.sin_zero));
    if (BoolServer == SERVER) {
        if (bind(this->SockFD, (struct sockaddr *) & this->UDPAddr, sizeof (this->UDPAddr)) < 0) {
            fprintf(stderr, "bind errno: %d - %s\n", errno, strerror(errno));
            return false;
        }
    }
    return true;
}

bool UDP::Close(void) {
    close(this->SockFD);
    return true;
}

bool UDP::Read(void * Buffer, size_t Size, int32_t *RecvLen) {
    if ((Buffer == NULL) || (RecvLen == NULL)) {
        return false;
    }
    int len;
    *RecvLen = recvfrom(this->SockFD, Buffer, Size, 0, (struct sockaddr *) & this->UDPAddr, (socklen_t*) & len);
    if (*RecvLen <= 0) {
        //        fprintf(stderr, "recfrom errno: %d - %s\n", errno, strerror(errno));
        return false;
    }
    return true;
}

bool UDP::Write(const void * Buffer, size_t Size, int32_t *SendLen) {
    if ((Buffer == NULL) || (SendLen == NULL)) {
        return false;
    }
    *SendLen = sendto(this->SockFD, Buffer, Size, 0, (struct sockaddr *) & this->UDPAddr, sizeof (this->UDPAddr));
    if ((size_t) * SendLen != Size) {
        //        fprintf(stderr, "sendto errno: %d - %s\n", errno, strerror(errno));
        return false;
    }
    return true;
}