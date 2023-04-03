#include <gtest/gtest.h>
#include "Fifo.hpp"
#include <string>

TEST(Fifo, ClientWithoutServer) {
  posix::FifoClient client;
  EXPECT_EQ(client.Open("/tmp/myfifo"), false);
}

TEST(Fifo, OpenServerClient) {
  posix::FifoServer server;
  posix::FifoClient client;

  EXPECT_EQ(server.Open("/tmp/myfifo"), true);
  EXPECT_EQ(client.Open("/tmp/myfifo"), true);
}

TEST(Fifo, ClientToServer) {
  posix::FifoServer server;
  posix::FifoClient client;

  EXPECT_EQ(server.Open("/tmp/myfifo"), true);
  EXPECT_EQ(client.Open("/tmp/myfifo"), true);

  std::string message_out = "Hello World!";
  ssize_t bytes = 0;
  EXPECT_EQ(client.Write(message_out.c_str() , message_out.size(), &bytes),
    true);

  char message_in[128] = {0};
  EXPECT_EQ(server.Read(message_in, sizeof(message_in), &bytes), true);

  EXPECT_STREQ(message_out.c_str(), message_in);

  EXPECT_EQ(client.Close(), true);
  EXPECT_EQ(server.Close(), true);
}
