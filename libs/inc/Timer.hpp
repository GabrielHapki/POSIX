#ifndef TIMER_H
#define TIMER_H

#include <chrono>

namespace posix{

class Timer {
private:
    std::chrono::time_point<std::chrono::steady_clock> beginTime;

public:
    Timer();
    ~Timer();
    void start();
    float get() const;
};

}

#endif /* TIMER_H */