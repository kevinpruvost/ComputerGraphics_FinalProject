/*****************************************************************//**
 * \file   Image2D.cpp
 * \brief  Source code of Image2D
 * 
 * \author Kevin Pruvost (pruvostkevin0@gmail.com)
 * \date   April, 27 2022
 *********************************************************************/
#include "Image2D.hpp"

Image2D::Image2D(const Mesh & mesh_, const Shader & shaderPoints_, const Shader & shaderWireframe_, const Shader & shaderFace_,
    const glm::vec2 & pos_, const GLfloat scale_)
    : pos{ pos_ }
    , scale{ scale_ }
    , mesh{ mesh_ }
    , shaderFace{ shaderFace_ }
    , shaderWireframe{ shaderWireframe_ }
    , shaderPoints{ shaderPoints_ }
{
}

Image2D::~Image2D()
{
}

void Image2D::SetTexture(const Texture & texture_)
{
    texture = texture_;
}
