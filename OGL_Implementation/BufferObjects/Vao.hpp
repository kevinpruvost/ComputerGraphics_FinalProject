/*****************************************************************//**
 * \file   Vao.hpp
 * \brief  Vao class, automatizes allocation/destruction
 * 
 * \author Kevin Pruvost (pruvostkevin0@gmail.com)
 * \date   April, 28 2022
 *********************************************************************/
#pragma once

// GLAD includes
#include <GLAD\glad.h>

class Vao
{
public:
    Vao();
    ~Vao();
    GLuint operator*() const;

private:
    GLuint __vao;
};
