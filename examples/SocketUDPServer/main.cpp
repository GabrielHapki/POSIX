#include <cstdlib>
#include <iostream>
#include "SocketUDP.hpp"

using namespace std;

int main(int argc, char** argv) {
    bool status = false;
    UDP server;

    uint8_t message_in[32] = {0};
    string message_out = "Hi";
    int32_t bytes = 0;

    status = server.Open(SERVER, "127.0.0.1", 5000, false);
    if (status == true){
        if (status == true){
            status = server.Read(message_in, sizeof(message_in), &bytes);
            cout << "Server:" << message_in << endl; 
            status = server.Write(message_out.c_str(), message_out.size(), &bytes);
        }
        status = server.Close();
    }

    return 0;
}

