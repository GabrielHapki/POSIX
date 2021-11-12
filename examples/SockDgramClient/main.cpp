#include <cstdlib>
#include <iostream>
#include <string>

#define SUDP

#ifdef SUDP
#include "Sudp.hpp"
#else
#include "SockDgram.hpp"
#endif

using namespace std;

int main(int argc, char** argv) {
    bool status = false;
#ifdef SUDP
    SudpClient client;
#else
    SockDgram client;
#endif

#ifdef SUDP
    std::string message = "Hello World!";
    Sudp::frame_t frame = {.size = message.size(), .buffer = std::vector<uint8_t>(message.data(), message.data() + message.size())};
    //Sudp::frame_t frame;
    //std::copy(message.begin(), message.end(), std::back_inserter(frame.buffer));
    //frame.size = message.size();

    if (client.Open(SockDgram::CLIENT, "127.0.0.1", 5000)) {
        while(1) {
            status = client.Send(frame);
            //status = client.Read(message_in, sizeof(message_in), &bytes);
            //cout << "Client:" << message_in << endl;
        }
        status = client.Close();
    }
#else
    string message_out = "Hello World!";
    uint8_t message_in[32] = {0};
    string message;
    ssize_t bytes = 0;

    if (client.Open(SockDgram::CLIENT, "127.0.0.1", 5000, SockDgram::NONBLOCK)) {
        while(1) {
            status = client.Write(message_out.c_str() , message_out.size(), &bytes);
            //status = client.Read(message_in, sizeof(message_in), &bytes);
            //cout << "Client:" << message_in << endl;
        }
        status = client.Close();
    }
#endif

    return 0;
}

