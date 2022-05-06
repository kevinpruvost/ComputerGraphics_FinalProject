/*****************************************************************//**
 * \file   OpenGL_Timer.hpp
 * \brief  Class made for performance measures
 * 
 * \author Kevin Pruvost (pruvostkevin0@gmail.com)
 * \date   April, 02 2022
 *********************************************************************/
#pragma once

// OpenGL includes
#include <GLAD\glad.h>

/**
 * @brief Timer based on rendering time GPU side
*/
class OpenGL_Timer
{
public:
    /**
     * @brief Timer Constructor
    */
    OpenGL_Timer();

    /**
     * @brief Starts Timer
    */
    void Start();

    /**
     * @brief Ends timer and returns nanoseconds passed
     * @return time in ns
    */
    GLuint64 End();

public:
    GLuint query;
};
