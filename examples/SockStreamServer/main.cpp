#include <cstdlib>
#include <iostream>
#include <strings.h>
#include "SockStreamServer.hpp"

using namespace std;

int main(int argc, char** argv) {
    SockStreamServer::status_t status = SockStreamServer::CLOSE;
    SockStreamServer server;

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
                    if (status == SockStreamServer::OK) {
                        cout << "Server: " << message_in << endl;
                    } else
                    if (status == SockStreamServer::NODATA) {
                        continue;
                    } else
                    if (status == SockStreamServer::CLOSE) {
                        break;
                    }
                    if (status == SockStreamServer::ERROR) {
                        std::cout << "Something very wrong! review your code!" << std::endl;
                    }
                }
            }
        }
        server.Close();
    }

    return 0;
}

