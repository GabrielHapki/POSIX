#include <cstdlib>
#include <iostream>
#include <string>
#include "MsgQueues.hpp"

using namespace std;

int main(int argc, char** argv) {
    string message_out = "Hello World!!!";

    try {
        posix::MsgQueuesSend mq("/Test_MQ");
        mq.send(message_out.c_str(), message_out.size(), 1);
    }
    catch(exception& e) {
         cerr << "Exception: " << e.what() << endl;
    }

    return 0;
}
