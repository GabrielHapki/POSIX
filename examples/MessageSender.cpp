#include <cstdlib>
#include <iostream>
#include <string>
#include "MsgQueues.hpp"

using namespace std;

int main(int argc, char** argv) {
    string message_out = "Hi!";
    uint8_t message_in[32] = {0};

    try {
        posix::MsgQueuesSend mq("/SoundPlayer_MQ");
        mq.send(message_out.c_str(), message_out.size(), 1);
    }
    catch(exception& e) {
         cerr << "Exception: " << e.what() << endl;
    }

    cout << "Receive: " << message_in << endl;

    return 0;
}