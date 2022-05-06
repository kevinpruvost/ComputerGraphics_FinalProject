/*****************************************************************//**
 * \file   Mesh_Image.cpp
 * \brief  Mesh Image source code
 * 
 * \author Kevin Pruvost (pruvostkevin0@gmail.com)
 * \date   April, 28 2022
 *********************************************************************/
#include "Mesh_Image.hpp"

// C++ includes
#include <memory>

Mesh_Image::Mesh_Image()
{
    GLfloat vertices[4][4] = {
            { 0.0, 0.0,   0.0, 0.0 },
            { 1000.0, 0.0,   1.0, 0.0 },
            { 0.0, 1000.0,   0.0, 1.0 },
            { 1000.0, 1000.0,   1.0, 1.0 }
    };
    glBindVertexArray(__facesVAO);
    // Fill mesh buffer
    glBindBuffer(GL_ARRAY_BUFFER, __facesVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    // Set mesh attributes
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);

    glBindVertexArray(__verticesVAO);
    // Fill mesh buffer
    glBindBuffer(GL_ARRAY_BUFFER, __verticesVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    // Set mesh attributes
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    __facesNVert = 4;
    __verticesNVert = 4;
}

Mesh_Image::~Mesh_Image()
{
    
}

GLuint Mesh_Image::GetFacesEBO() const
{
    return 0;
}

bool Mesh_Image::IsUsingEBO() const
{
    return false;
}

Mesh_Base::DrawMode Mesh_Image::GetDrawMode() const
{
    return DrawMode::DrawArrays;
}