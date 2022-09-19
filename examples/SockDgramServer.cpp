#include <cstdlib>
#include <iostream>
#include <string>

#define SUDP

#ifdef SUDP
#include "Sudp.hpp"
#else
#include "SockDgram.hpp"
#endif

using namespace std;

int main(int argc, char** argv) {
    bool status = false;
#ifdef SUDP
    posix::SudpServer server;
#else
    posix::SockDgram server;
#endif

#ifdef SUDP
    posix::Sudp::frame_t frame;
    frame.buffer = std::vector<uint8_t>(500*1024);

    if (server.Open(posix::SockDgram::SERVER, "127.0.0.1", 5000)) {
        while(1) {
            status = server.Recv(frame);
            if (status) {
                std::string message((char *)frame.buffer.data(), frame.size);
                cout << "Server: " << message << endl;
            }
//                status = server.Write(message_out.c_str(), message_out.size(), &bytes);
        }
        status = server.Close();
    }
#else
    uint8_t message_in[32] = {0};
    string message_out = "Hi";
    ssize_t bytes = 0;

    if (server.Open(SockDgram::SERVER, "127.0.0.1", 5000, SockDgram::NONBLOCK)) {
        while(1) {
            status = server.Read(message_in, sizeof(message_in), &bytes);
            if (status)
                cout << "Server: " << message_in << endl;
//                status = server.Write(message_out.c_str(), message_out.size(), &bytes);
        }
        status = server.Close();
    }
#endif

    return 0;
}

