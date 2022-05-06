/*****************************************************************//**
 * \file   Vbo.hpp
 * \brief  Vbo class, automatizes allocation/destruction
 * 
 * \author Kevin Pruvost (pruvostkevin0@gmail.com)
 * \date   April, 28 2022
 *********************************************************************/
#pragma once

// GLAD includes
#include <GLAD\glad.h>

class Vbo
{
public:
    Vbo();
    ~Vbo();
    GLuint operator*() const;

private:
    GLuint __vbo;
};
