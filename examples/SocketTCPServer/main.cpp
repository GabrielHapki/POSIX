#include <cstdlib>
#include <iostream>
#include "SocketTCPServer.hpp"

using namespace std;

int main(int argc, char** argv) {
    SocketTCP::STATUS status = SocketTCP::STATUS::CLOSE;
    SocketTCP::Server server;

    uint8_t message_in[32] = {0};
    string message_out = "Hi";
    int32_t bytes = 0;

    status = server.Open(5000);
    if (status == SocketTCP::STATUS::OPEN){
        status = server.Accept();
        if (status == SocketTCP::STATUS::OPEN){
            status = server.Read(message_in, sizeof(message_in), &bytes);
            cout << "Server:" << message_in << endl; 
            status = server.Write(message_out.c_str(), message_out.size(), &bytes);
        }
        status = server.Close();
    }

    return 0;
}

