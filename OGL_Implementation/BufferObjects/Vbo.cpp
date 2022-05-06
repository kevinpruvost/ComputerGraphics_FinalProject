/*****************************************************************//**
 * \file   Vbo.cpp
 * \brief  Vbo source code
 * 
 * \author Kevin Pruvost (pruvostkevin0@gmail.com)
 * \date   April, 28 2022
 *********************************************************************/
#include "Vbo.hpp"

Vbo::Vbo()
{
    glGenBuffers(1, &__vbo);
}

Vbo::~Vbo()
{
    glDeleteBuffers(1, &__vbo);
}

GLuint Vbo::operator*() const
{
    return __vbo;
}
