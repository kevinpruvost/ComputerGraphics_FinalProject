/*****************************************************************//**
 * \file   Image2D.hpp
 * \brief  2D Image displayed directly on the camera perspective
 * 
 * \author Kevin Pruvost (pruvostkevin0@gmail.com)
 * \date   April, 27 2022
 *********************************************************************/
#pragma once

// Project includes
#include "OGL_Implementation\Mesh\Mesh.hpp"
#include "OGL_Implementation\Shader\Shader.hpp"
#include "OGL_Implementation\EntityAttribute\Texture.hpp"

/**
 * @brief Texture drawn in an orthogonal projection.
*/
class Image2D
{
public:
    Image2D(const Mesh & mesh, const Shader & _shaderPoints, const Shader & _shaderWireframe, const Shader & _shaderFace, const glm::vec2 & pos_, const GLfloat scale_);
    ~Image2D();

    void SetTexture(const Texture & texture_);

public:
    glm::vec2 pos;
    GLfloat scale;
    glm::vec3 color;
    Shader shaderFace, shaderWireframe, shaderPoints;
    Mesh mesh;
    Texture texture;
};
