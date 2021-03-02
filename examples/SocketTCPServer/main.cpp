#include <cstdlib>
#include "SocketTCPServer.hpp"

using namespace std;

int main(int argc, char** argv) {
    SocketTCP::STATUS status = SocketTCP::STATUS::CLOSE;
    SocketTCP::Server server;

    uint8_t message[32] = {0};
    uint8_t Hi[] = "Hi";
    int32_t bytes = 0;

    status = server.Open(5000);
    status = server.Accept();
    status = server.Read(message, sizeof(message), &bytes);
    status = server.Write(Hi, sizeof(Hi), &bytes);
    status = server.Close();

    return 0;
}

