#ifndef TIMER_H
#define TIMER_H

#include <chrono>

class Timer {
private:
    std::chrono::time_point<std::chrono::steady_clock> beginTime;

public:
    Timer() : beginTime(std::chrono::steady_clock::now())
    {

    }

    ~Timer()
    {

    }

    void start()
    {
        beginTime = std::chrono::steady_clock::now();
    }

    float get() const
    {
        std::chrono::duration<float> elapsedTime = std::chrono::steady_clock::now() - beginTime;
        return elapsedTime.count();        
    }
};

#endif /* TIMER_H */