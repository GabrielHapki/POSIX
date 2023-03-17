#include <gtest/gtest.h>
#include "SockDgram.hpp"
#include <string>

TEST(SockDgram, Init) {
  posix::SockDgram *client = nullptr;

  EXPECT_NO_THROW({
    try {
      client = new posix::SockDgram();
    }
    catch(std::exception& e) {
      std::cerr << "Exception: " << e.what() << std::endl;
      throw;
    }
  });  

  EXPECT_NE(client, nullptr);

  delete client;
}

TEST(SockDgram, ClientToServer) {
  posix::SockDgram *client = nullptr;
  posix::SockDgram *server = nullptr;

  EXPECT_NO_THROW({
    try {
      client = new posix::SockDgram();
    }
    catch(std::exception& e) {
      std::cerr << "Exception: " << e.what() << std::endl;
      throw;
    }
  });

  EXPECT_NO_THROW({
    try {
      server = new posix::SockDgram();
    }
    catch(std::exception& e) {
      std::cerr << "Exception: " << e.what() << std::endl;
      throw;
    }
  });

  EXPECT_EQ(client->Open(posix::SockDgram::CLIENT, "127.0.0.1", 5000, posix::SockDgram::NONBLOCK), true);
  EXPECT_EQ(server->Open(posix::SockDgram::SERVER, "127.0.0.1", 5000, posix::SockDgram::NONBLOCK), true);

  std::string message_out = "Hello World!!!";
  ssize_t bytes = 0;
  EXPECT_EQ(client->Write(message_out.c_str(), message_out.size(), &bytes), true);
  EXPECT_EQ(message_out.size(), bytes);

  char message_in[32] = {0};
  EXPECT_EQ(server->Read(message_in, sizeof(message_in), &bytes), true);

  EXPECT_EQ(message_out.size(), strlen(message_in));
  EXPECT_STREQ(message_out.c_str(), message_in);

  EXPECT_EQ(client->Close(), true);
  EXPECT_EQ(server->Close(), true);

  delete client;
  delete server;
}
