/*****************************************************************//**
 * \file   Vao.cpp
 * \brief  Vao source code
 * 
 * \author Kevin Pruvost (pruvostkevin0@gmail.com)
 * \date   April, 28 2022
 *********************************************************************/
#include "Vao.hpp"

Vao::Vao()
{
    glGenVertexArrays(1, &__vao);
}

Vao::~Vao()
{
    glDeleteVertexArrays(1, &__vao);
}

GLuint Vao::operator*() const
{
    return __vao;
}
