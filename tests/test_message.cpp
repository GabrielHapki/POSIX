#include <gtest/gtest.h>
#include "MsgQueues.hpp"
#include <string>

TEST(Message, ReceiverInit) {
  posix::MsgQueuesReceive *mq = nullptr;

  EXPECT_NO_THROW({
    try {
      mq = new posix::MsgQueuesReceive("/Test_MQ", false);
    }
    catch(std::exception& e) {
      std::cerr << "Exception: " << e.what() << std::endl;
      throw;
    }
  });  

  EXPECT_NE(mq, nullptr);

  delete mq;
}

TEST(Message, SenderInit) {
  posix::MsgQueuesSend *mq = nullptr;

  EXPECT_NO_THROW({
    try {
      mq = new posix::MsgQueuesSend("/Test_MQ");
    }
    catch(std::exception& e) {
      std::cerr << "Exception: " << e.what() << std::endl;
      throw;
    }
  });  

  EXPECT_NE(mq, nullptr);

  delete mq;
}

struct myMessage {
    char data[32];
    ssize_t size;
    unsigned int priority;
};

TEST(Message, SenderToReceiver) {
  posix::MsgQueuesReceive *mqr = nullptr;
  posix::MsgQueuesSend *mqs = nullptr;

  EXPECT_NO_THROW({
    try {
      mqr = new posix::MsgQueuesReceive("/Test_MQ", false);
    }
    catch(std::exception& e) {
      std::cerr << "Exception: " << e.what() << std::endl;
      throw;
    }
  });  
  EXPECT_NE(mqr, nullptr);

  std::string message_out = "Hello World!!!";
  EXPECT_NO_THROW({
    try {
      mqs = new posix::MsgQueuesSend("/Test_MQ");
      mqs->send(message_out.c_str(), message_out.size(), 1);
    }
    catch(std::exception& e) {
      std::cerr << "Exception: " << e.what() << std::endl;
      throw;
    }
  });  
  EXPECT_NE(mqs, nullptr);

  myMessage message = {0};
  EXPECT_EQ(mqr->receive(message.data, sizeof(message.data), &message.size, &message.priority), true);

  EXPECT_EQ(message.priority, 1);
  EXPECT_EQ(message.size, message_out.size());
  EXPECT_STREQ(message_out.c_str(), message.data);

  delete mqr;
  delete mqs;
}

class EventReceiver : public posix::MsgQueuesReceive{
    private:
        myMessage &message;
    public:
        EventReceiver(const char *key, myMessage &pMessage) :
        MsgQueuesReceive(key, true),
        message(pMessage) {}
        inline void event()
        {
            if (receive(message.data, sizeof(message.data), &message.size, &message.priority)) {
                // std::cout << "Event Receive: " << std::to_string(message.size) << "bytes, " << message.data << ", priority " << std::to_string(message.priority) << std::endl;
            }
        }
};

TEST(Message, SenderToReceiverEvent) {
  EventReceiver *mqr = nullptr;
  posix::MsgQueuesSend *mqs = nullptr;
  myMessage message = {0};

  EXPECT_NO_THROW({
    try {
      mqr = new EventReceiver("/Test_MQ", message);
    }
    catch(std::exception& e) {
      std::cerr << "Exception: " << e.what() << std::endl;
      throw;
    }
  });  
  EXPECT_NE(mqr, nullptr);

  std::string message_out = "Hello World!!!";
  EXPECT_NO_THROW({
    try {
      mqs = new posix::MsgQueuesSend("/Test_MQ");
      mqs->send(message_out.c_str(), message_out.size(), 1);
    }
    catch(std::exception& e) {
      std::cerr << "Exception: " << e.what() << std::endl;
      throw;
    }
  });  
  EXPECT_NE(mqs, nullptr);

  usleep(10000);

  EXPECT_EQ(message.priority, 1);
  EXPECT_EQ(message.size, message_out.size());
  EXPECT_STREQ(message_out.c_str(), message.data);

  delete mqr;
  delete mqs;
}
