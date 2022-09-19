#include <cstdlib>
#include <iostream>
#include <string>
#include "MsgQueues.hpp"

using namespace std;

int main(int argc, char** argv) {
    string message_out = "Hello World!";
    uint8_t message_in[32] = {0};
    ssize_t bytes = 0;
    unsigned int priority = 0;

    try {
        posix::MsgQueuesReceive mq(posix::MsgQueue::SOUNDPLAYER);
        mq.receive(message_in, sizeof(message_in), &bytes, &priority);
    }
    catch(exception& e) {
         cerr << "Exception: " << e.what() << endl;
    }

    cout << "Receive: " << message_in << endl;

    return 0;
}