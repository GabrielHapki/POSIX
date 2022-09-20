#include <cstdlib>
#include <iostream>
#include <string>
#include "MsgQueues.hpp"

using namespace std;

int main(int argc, char** argv) {
    uint8_t message_in[32] = {0};
    ssize_t bytes = 0;
    unsigned int priority = 0;

    try {
        posix::MsgQueuesReceive mq("/Test_MQ");
        if (mq.receive(message_in, sizeof(message_in), &bytes, &priority))
            cout << "Receive: " << std::to_string(bytes) << "bytes, " << message_in << ", priority " << std::to_string(priority) << endl;
    }
    catch(exception& e) {
         cerr << "Exception: " << e.what() << endl;
    }

    return 0;
}
