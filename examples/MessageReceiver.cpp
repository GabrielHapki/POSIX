#include <cstdlib>
#include <iostream>
#include <string>
#include "MsgQueues.hpp"

#include <unistd.h>

struct myMessage {
    uint8_t data[32];
    ssize_t size;
    unsigned int priority;
};

int main(int argc, char** argv) {
    posix::MsgQueuesReceive *mq = nullptr;

    try {
        mq = new posix::MsgQueuesReceive("/Test_MQ", false);
    } catch(std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
        return 0;
    }

    while(1) {
        myMessage message = {0};
    
        if (mq->receive(message.data, sizeof(message.data), &message.size, &message.priority)) {
            std::cout << "Receive: " << std::to_string(message.size) << "bytes, " << message.data << ", priority " << std::to_string(message.priority) << std::endl;
        } else
            sleep(5);
    }

    delete mq;

    return 0;
}
