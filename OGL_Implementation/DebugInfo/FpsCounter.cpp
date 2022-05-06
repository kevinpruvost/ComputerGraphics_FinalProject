/*****************************************************************//**
 * \file   FpsCounter.cpp
 * \brief  Fps Counter Source Code
 * 
 * \author Kevin Pruvost (pruvostkevin0@gmail.com)
 * \date   April, 03 2022
 *********************************************************************/
#include "FpsCounter.hpp"

static uint16_t lastFpsCount = 0, framesCount = 0;
static double totalDeltaTime = 0.0f;

uint16_t GetFpsCount(double deltaTime, double refreshTime, int frames)
{
    framesCount += frames;
    totalDeltaTime += deltaTime;
    if (totalDeltaTime >= refreshTime)
    {
        lastFpsCount = framesCount / totalDeltaTime;
        framesCount = 0;
        totalDeltaTime = 0.0f;
    }
    return lastFpsCount;
}