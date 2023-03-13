#include "Timer.hpp"

namespace posix{

Timer::Timer(): beginTime(std::chrono::steady_clock::now())
{

}

Timer::~Timer()
{

}

void Timer::start()
{
    beginTime = std::chrono::steady_clock::now();
}

float Timer::get() const
{
    std::chrono::duration<float> elapsedTime = std::chrono::steady_clock::now() - beginTime;
    return elapsedTime.count();        
}

}
