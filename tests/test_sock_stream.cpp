#include <gtest/gtest.h>
#include "SockStreamClient.hpp"
#include "SockStreamServer.hpp"
#include <thread>
#include <string>

TEST(SockStream, ClientInit) {
  posix::SockStreamClient *client = nullptr;

  EXPECT_NO_THROW({
    try {
      client = new posix::SockStreamClient();
    }
    catch(std::exception& e) {
      std::cerr << "Exception: " << e.what() << std::endl;
      throw;
    }
  });  

  EXPECT_NE(client, nullptr);

  delete client;
}

TEST(SockStream, ServerInit) {
  posix::SockStreamServer *server = nullptr;

  EXPECT_NO_THROW({
    try {
      server = new posix::SockStreamServer();
    }
    catch(std::exception& e) {
      std::cerr << "Exception: " << e.what() << std::endl;
      throw;
    }
  });  

  EXPECT_NE(server, nullptr);

  delete server;
}

class myTest
{
  public:
    myTest() : server(nullptr), client(nullptr), run(true)
    {
      EXPECT_NO_THROW({
        try {
          server = new posix::SockStreamServer();
        }
        catch(std::exception& e) {
          std::cerr << "Exception: " << e.what() << std::endl;
          throw;
        }
      });  

      EXPECT_NO_THROW({
        try {
          client = new posix::SockStreamClient();
        }
        catch(std::exception& e) {
          std::cerr << "Exception: " << e.what() << std::endl;
          throw;
        }
      });

      handleThread = std::thread(&myTest::Thread, this);
      handleThread.detach();
      
      usleep(10000);

      EXPECT_EQ(client->Open("127.0.0.1", 5000), true);

      std::string message_out = "Hello World!!!";
      ssize_t bytes = 0;
      EXPECT_EQ(client->Write(message_out.c_str() , message_out.size(), &bytes), posix::SockStreamClient::status_t::OK);
      
      usleep(10000);
      run = false;

      EXPECT_EQ(message_out.size(), strlen(message_in));
      EXPECT_STREQ(message_out.c_str(), message_in);
    }

    ~myTest()
    {
      EXPECT_EQ(client->Close(), true);
      EXPECT_EQ(server->Close(), true);

      delete client;
      delete server;
    }

    void Thread()
    {
      posix::SockStreamServer::status_t status = posix::SockStreamServer::CLOSE;
      ssize_t bytes = 0;
      EXPECT_EQ(server->Open(5000, 1), true);

      while(run) {
        if (server->Accept()) {
          bzero(message_in, sizeof(message_in));
          status = server->Read(message_in, sizeof(message_in), &bytes);
          if (status == posix::SockStreamServer::OK) {
              // std::cout << "Server: " << message_in << std::endl;
          } else
          if (status == posix::SockStreamServer::NODATA) {
              continue;
          } else
          if (status == posix::SockStreamServer::CLOSE) {
              break;
          }
          if (status == posix::SockStreamServer::ERROR) {
              std::cout << "Something very wrong! review your code!" << std::endl;
          }
        }
      }
    }

  private:
    posix::SockStreamServer *server;
    posix::SockStreamClient *client;
    std::thread handleThread;
    char message_in[128];
    bool run;
};

TEST(SockStream, ClientToServer) {
  myTest *mTest = new myTest();
  delete mTest;
}
