#include <cstdlib>
#include <iostream>
#include <string>
#include "SocketTCPClient.hpp"

using namespace std;

int main(int argc, char** argv) {
    SocketTCP::STATUS status = SocketTCP::CLOSE;
    SocketTCP::Client client;

    string message_out = "Hello World!";
    uint8_t message_in[32] = {0};
    string message;
    int32_t bytes = 0;

    status = client.Open("127.0.0.1", 5000);
    if (status == SocketTCP::STATUS::OPEN){
        status = client.Write(message_out.c_str() , message_out.size(), &bytes);
        status = client.Read(message_in, sizeof(message_in), &bytes);
        cout << "Client:" << message_in << endl;
        status = client.Close();
    }

    return 0;
}

