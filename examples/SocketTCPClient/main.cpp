#include <cstdlib>
#include "SocketTCPClient.hpp"

using namespace std;

#define ServerAddress "127.0.0.1"

int main(int argc, char** argv) {
    SocketTCP::STATUS status = SocketTCP::CLOSE;
    SocketTCP::Client client;

    uint8_t message[] = "Hello World!";
    int32_t bytes = 0;

    status = client.Open("127.0.0.1", 5000);
    status = client.Write(message, sizeof(message), &bytes);
    status = client.Read(message, sizeof(message), &bytes);
    status = client.Close();

    return 0;
}

