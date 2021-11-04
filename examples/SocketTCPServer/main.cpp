#include <cstdlib>
#include <iostream>
#include <strings.h>
#include "SocketTCPServer.hpp"

using namespace std;

int main(int argc, char** argv) {
    Server::status_t status = Server::CLOSE;
    Server server;

    uint8_t message_in[128] = {0};
    string message_out = "Hi";
    ssize_t bytes = 0;

    if (server.Open(5000, 1)){
        while(1) {
            if (server.Accept()) {
                while(1) {
                    bzero(message_in, sizeof(message_in));
                    status = server.Read(message_in, sizeof(message_in), &bytes);
                    //status = server.Write(message_out.c_str(), message_out.size(), &bytes);
                    if (status == Server::OK) {
                        cout << "Server: " << message_in << endl;
                    } else
                    if (status == Server::NODATA) {
                        continue;
                    } else
                    if (status == Server::CLOSE) {
                        break;
                    }
                    if (status == Server::ERROR) {
                        std::cout << "Something very wrong! review your code!" << std::endl;
                    }
                }
            }
        }
        server.Close();
    }

    return 0;
}

