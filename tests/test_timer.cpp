#include <gtest/gtest.h>
#include "Timer.hpp"

TEST(Timer, Init) {
  posix::Timer *timer = nullptr;

  EXPECT_NO_THROW({
    try {
      timer = new posix::Timer();
    }
    catch(std::exception& e) {
      std::cerr << "Exception: " << e.what() << std::endl;
      throw;
    }
  });

  EXPECT_NE(timer, nullptr);

  delete timer;
}

TEST(Timer, Test) {
  posix::Timer *timer = nullptr;

  EXPECT_NO_THROW({
    try {
      timer = new posix::Timer();
    }
    catch(std::exception& e) {
      std::cerr << "Exception: " << e.what() << std::endl;
      throw;
    }
  });

  EXPECT_NE(timer, nullptr);

  uint32_t delay = 1000;
  timer->start();
  usleep(delay);
  EXPECT_NEAR(timer->get(), delay/1000000.f, 1e-4);

  delete timer;
}
