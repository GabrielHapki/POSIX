#include <cstdlib>
#include <iostream>
#include <string>
#include "Fifo.hpp"

using namespace std;

int main(int argc, char** argv) {
    bool status = false;
    posix::FifoClient client;

    string message_out = "Hello World!";
    uint8_t message_in[32] = {0};
    string message;
    ssize_t bytes = 0;

    if (client.Open("/tmp/myfifo")) {
        while(1) {
            status = client.Write(message_out.c_str() , message_out.size(), &bytes);
            //status = client.Read(message_in, sizeof(message_in), &bytes);
            //cout << "Client:" << message_in << endl;
        }
        status = client.Close();
    }

    return 0;
}

