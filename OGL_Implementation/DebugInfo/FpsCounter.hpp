/*****************************************************************//**
 * \file   FpsCounter.hpp
 * \brief  Fps Counter Code
 * 
 * \author Kevin Pruvost (pruvostkevin0@gmail.com)
 * \date   April, 03 2022
 *********************************************************************/
#pragma once

// C++ includes
#include <cstdint>

/**
 * @brief Returns Fps Count
 * @param deltaTime (in seconds)
 * @param refreshTime (in seconds)
 * @param count of frames passed for 'deltaTime'
 * @return fps count
*/
uint16_t GetFpsCount(double deltaTime, double refreshTime = 0.5f, int frames = 1);