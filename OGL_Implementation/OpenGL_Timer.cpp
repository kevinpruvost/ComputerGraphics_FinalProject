/*****************************************************************//**
 * \file   OpenGL_Timer.cpp
 * \brief  Source code of OpenGL_Timer class
 * 
 * \author Kevin Pruvost (pruvostkevin0@gmail.com)
 * \date   April, 03 2022
 *********************************************************************/
#include "OpenGL_Timer.hpp"

OpenGL_Timer::OpenGL_Timer()
    : query{ 0 }
{
}

void OpenGL_Timer::Start()
{
    glGenQueries(1, &query);
    glBeginQuery(GL_TIME_ELAPSED, query);
}

GLuint64 OpenGL_Timer::End()
{
    glEndQuery(GL_TIME_ELAPSED);

    // retrieving the recorded elapsed time
    // wait until the query result is available
    GLint done = 0;
    while (!done)
    {
        glGetQueryObjectiv(query,
            GL_QUERY_RESULT_AVAILABLE,
            &done);
    }
    GLuint64 elapsedTime;
    glGetQueryObjectui64v(query, GL_QUERY_RESULT, &elapsedTime);
    return elapsedTime;
}
