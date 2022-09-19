#include <cstdlib>
#include <iostream>
#include "Fifo.hpp"
#include <string.h>

using namespace std;

int main(int argc, char** argv) {
    bool status = false;
    posix::FifoServer server;

    uint8_t message_in[128] = {0};
    string message_out = "Hi";
    ssize_t bytes = 0;

    if (server.Open("/tmp/myfifo")) {
        while(1) {
            bzero(message_in, sizeof(message_in));
            status = server.Read(message_in, sizeof(message_in), &bytes);
            if (status)
                cout << "Server: " << message_in << endl;
//                status = server.Write(message_out.c_str(), message_out.size(), &bytes);
        }
        status = server.Close();
    }

    return 0;
}

