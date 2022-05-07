/*****************************************************************//**
 * \file   Timer.hpp
 * \brief  Timer class
 * 
 * \author Kevin Pruvost (pruvostkevin0@gmail.com)
 * \date   May, 07 2022
 *********************************************************************/
#pragma once

// Project includes
#include <chrono>

class Timer
{
public:
    Timer();
    ~Timer();

    void Start();
    double GetSTime();
    double GetMsTime();

public:
    std::chrono::steady_clock::time_point start;
};
