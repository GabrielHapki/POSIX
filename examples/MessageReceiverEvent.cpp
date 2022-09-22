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

class EventReceiver : public posix::MsgQueuesReceive{
    private:
        myMessage &message;
    public:
        EventReceiver(const char *key, myMessage &pMessage) :
        MsgQueuesReceive(key, true),
        message(pMessage) {}
        inline void event();
};

inline void EventReceiver::event()
{
    if (receive(message.data, sizeof(message.data), &message.size, &message.priority)) {
        std::cout << "Event Receive: " << std::to_string(message.size) << "bytes, " << message.data << ", priority " << std::to_string(message.priority) << std::endl;
    }
}

int main(int argc, char** argv) {
    EventReceiver *mq = nullptr;
    myMessage message = {0};
    
    try {
        mq = new EventReceiver("/Test_MQ", message);
    } catch(std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
        return 0;
    }

    while(1) {
        if (mq->receive(message.data, sizeof(message.data), &message.size, &message.priority)) {
            std::cout << "New Receive: " << std::to_string(message.size) << "bytes, " << message.data << ", priority " << std::to_string(message.priority) << std::endl;
        } else {
            std::cout << "Last Receive: " << std::to_string(message.size) << "bytes, " << message.data << ", priority " << std::to_string(message.priority) << std::endl;
            sleep(5);
        }
    }

    delete mq;

    return 0;
}
