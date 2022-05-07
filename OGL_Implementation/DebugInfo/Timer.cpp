/*****************************************************************//**
 * \file   Timer.cpp
 * \brief  Timer source code
 * 
 * \author Kevin Pruvost (pruvostkevin0@gmail.com)
 * \date   May, 07 2022
 *********************************************************************/
#include "Timer.hpp"

Timer::Timer()
    : start{ std::chrono::steady_clock::now() }
{
}

Timer::~Timer()
{
}

void Timer::Start()
{
    start = std::chrono::steady_clock::now();
}

double Timer::GetSTime()
{
    auto end = std::chrono::steady_clock::now();
    return std::chrono::duration_cast<std::chrono::seconds>(end - start).count();
}

double Timer::GetMsTime()
{
    auto end = std::chrono::steady_clock::now();
    return std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
}
