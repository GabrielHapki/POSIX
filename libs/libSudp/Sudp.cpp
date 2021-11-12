#include "Sudp.hpp"

bool Sudp::Open(const SockDgram::role_t role, const char *ip, const uint32_t port) {
    return sock.Open(role, ip, port, SockDgram::NONBLOCK);
}

bool Sudp::Close(void) {
    return sock.Close();
}

bool SudpServer::Recv(frame_t &frame)
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

bool SudpClient::Send(const frame_t frame)
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