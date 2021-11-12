#ifndef SUDP_H
#define SUDP_H

#include "SockDgram.hpp"
#include <vector>

class Sudp {
    protected:
        SockDgram sock;

    public:
        struct frame_t {
            uint32_t size;
            std::vector<uint8_t> buffer;
        };
        Sudp(void) {}
        virtual ~Sudp(void) {}
        bool Open(const SockDgram::role_t role, const char *ip, const uint32_t port);
        bool Close(void);
};

class SudpServer : public Sudp{
    public:
        SudpServer(void) {}
        virtual ~SudpServer(void) {}
        bool Recv(frame_t &frame);
};

class SudpClient : public Sudp{
    public:
        SudpClient(void) {}
        virtual ~SudpClient(void) {}
        bool Send(const frame_t frame);
};

#endif /* SUDP_H */

