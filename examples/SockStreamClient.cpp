#include <cstdlib>
#include <iostream>
#include <string>
#include <unistd.h>
#include "SockStreamClient.hpp"

using namespace std;

int main(int argc, char** argv) {
    posix::SockStreamClient::status_t status = posix::SockStreamClient::CLOSE;
    posix::SockStreamClient client;

    uint8_t message_in[128] = {0};
    string message_out = "Hello World! ";
    uint32_t count = 0;
    string message;
    ssize_t bytes = 0;

    while(1) {
        if (client.Open("127.0.0.1", 5000)){
            //for (auto i=0; i < 10; i++) {
            while(1) {
                message_out.clear();
                message_out = "Hello World! " + std::to_string(count);
                status = client.Write(message_out.c_str() , message_out.size(), &bytes);
                //status = client.Read(message_in, sizeof(message_in), &bytes);
                if (status == posix::SockStreamClient::OK) {
                    if (bytes > 0)
                        cout << "Client: " << message_in << endl;
                    count++;
                } else
                if (status == posix::SockStreamClient::NODATA) {
                    continue;
                } else
                if (status == posix::SockStreamClient::CLOSE) {
                    break;
                }
                if (status == posix::SockStreamClient::ERROR) {
                    std::cout << "Something very wrong! review your code!" << std::endl;
                }
                usleep(250000);
            }
        }
        sleep(1);
    }

    return 0;
}

