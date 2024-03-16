#pragma once
#include <chrono>
#include <iostream>

class Timer
{
private:
    std::chrono::milliseconds start;
    std::chrono::milliseconds end;

public:
    Timer();
    ~Timer();

    void print(std::string pretext);
};

Timer::Timer()
{
    start = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());
}

Timer::~Timer()
{

}

void Timer::print(std::string pretext)
{
    end = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());
    std::cout << pretext << (end - start).count() << "ms" << std::endl;
}
