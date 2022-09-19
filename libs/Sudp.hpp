#ifndef SUDP_H
#define SUDP_H

#include "SockDgram.hpp"
#include <vector>

namespace posix{

class Sudp {
    protected:
        SockDgram sock;

    public:
        struct frame_t {
            uint32_t size;
            std::vector<uint8_t> buffer;
        };
        Sudp(void)
        {

        }

        virtual ~Sudp(void)
        {

        }

        bool Open(const SockDgram::role_t role, const char *ip, const uint32_t port)
        {
            return sock.Open(role, ip, port, SockDgram::NONBLOCK);
        }

        bool Close(void)
        {
            return sock.Close();
        }
};

class SudpServer : public Sudp{
    public:
        SudpServer(void)
        {

        }

        virtual ~SudpServer(void)
        {

        }

        bool Recv(frame_t &frame)
        {
            ssize_t bytes = 0;
            if (!sock.Read(&frame.size, sizeof(frame.size), &bytes))
                return false;
            if (sizeof(frame.size) != bytes)
                return false;
            if (frame.size <= frame.buffer.size()) {
                if (!sock.Read(frame.buffer.data(), frame.size, &bytes))
                    return false;
                if (frame.size != bytes)
                    return false;
            } else {
                return false;
            }
            return true;
        }
};

class SudpClient : public Sudp{
    public:
        SudpClient(void)
        {

        }

        virtual ~SudpClient(void)
        {

        }

        bool Send(const frame_t frame)
        {
            ssize_t bytes = 0;
            if (!sock.Write(&frame.size, sizeof(frame.size), &bytes))
                return false;
            if (frame.size <= frame.buffer.size()) {
                if (sock.Write(frame.buffer.data(), frame.size, &bytes)) {
                    return true;
                }
            }
            return false;
        }
};

}

#endif /* SUDP_H */
