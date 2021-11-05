#include <cstdlib>
#include <iostream>
#include <string>
#include "SockDgram.hpp"

using namespace std;

int main(int argc, char** argv) {
    bool status = false;
    UDP client;

    string message_out = "Hello World!";
    uint8_t message_in[32] = {0};
    string message;
    ssize_t bytes = 0;

    if (client.Open(UDP::CLIENT, "127.0.0.1", 5000, UDP::NONBLOCK)) {
        while(1) {
            status = client.Write(message_out.c_str() , message_out.size(), &bytes);
            //status = client.Read(message_in, sizeof(message_in), &bytes);
            //cout << "Client:" << message_in << endl;
        }
        status = client.Close();
    }

    return 0;
}

