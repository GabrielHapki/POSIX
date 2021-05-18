#include <cstdlib>
#include <iostream>
#include <string>
#include "MessageQueues.hpp"

using namespace std;

int main(int argc, char** argv) {
    string message_out = "Hello World!";
    uint8_t message_in[32] = {0};
    int32_t bytes = 0;

    try {
        MessageQueues receiver(1000);
        receiver.Receive(message_in, sizeof(message_in), &bytes);
        //receiver.Send(message_out.c_str(), message_out.size());
    }
    catch(exception& e) {
         cerr << "Exception: " << e.what() << endl;
    }

    cout << "Receive: " << message_in << endl;

    return 0;
}