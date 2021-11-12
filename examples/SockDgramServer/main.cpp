#include <cstdlib>
#include <iostream>
#include "SockDgram.hpp"

using namespace std;

int main(int argc, char** argv) {
    bool status = false;
    SockDgram server;

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

    return 0;
}

