/*****************************************************************//**
 * \file   Mesh_Custom.cpp
 * \brief  Mesh_Custom source code
 * 
 * \author Kevin Pruvost (pruvostkevin0@gmail.com)
 * \date   May, 01 2022
 *********************************************************************/
#include "Mesh_Custom.hpp"

// C++ includes
#include <stdexcept>

Mesh_Custom::Mesh_Custom(const std::vector<VertexNormalTexture> & vertices_)
    : __vertices{ vertices_ }
{
    glBindVertexArray(__facesVAO);
    // Fill mesh buffer
    glBindBuffer(GL_ARRAY_BUFFER, __facesVBO);
    glBufferData(GL_ARRAY_BUFFER, __vertices.size() * sizeof(VertexNormalTexture), __vertices.data(), GL_STATIC_DRAW);
    // Set mesh attributes
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), 0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid *)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid *)(6 * sizeof(GLfloat)));

    glBindVertexArray(__verticesVAO);
    // Fill mesh buffer
    glBindBuffer(GL_ARRAY_BUFFER, __verticesVBO);
    glBufferData(GL_ARRAY_BUFFER, __vertices.size() * sizeof(VertexNormalTexture), __vertices.data(), GL_STATIC_DRAW);
    // Set mesh attributes
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), 0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid *)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid *)(6 * sizeof(GLfloat)));

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    __facesNVert = __verticesNVert = __vertices.size();
}

Mesh_Custom::~Mesh_Custom()
{
}

GLuint Mesh_Custom::GetFacesEBO() const
{
    return 0;
}

bool Mesh_Custom::IsUsingEBO() const
{
    return false;
}

Mesh_Base::DrawMode Mesh_Custom::GetDrawMode() const
{
    return Mesh_Base::DrawMode::DrawArrays;
}

void Mesh_Custom::ModifyVertex(const unsigned int index, const VertexNormalTexture & newVertex)
{
    if (index >= __vertices.size()) throw std::runtime_error("Vertex out of bound");

    __vertices[index] = newVertex;

    ReassignVertex();
}

void Mesh_Custom::ModifyVertices(const std::vector<VertexNormalTexture> & vertices)
{
    __vertices = vertices;

    ReassignVertex();
    
    __facesNVert = __verticesNVert = __vertices.size();
}

inline void Mesh_Custom::ReassignVertex()
{
    glBindVertexArray(__verticesVAO);
    glBindBuffer(GL_ARRAY_BUFFER, __verticesVAO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, __vertices.size() * sizeof(VertexNormalTexture), __vertices.data());

    glBindVertexArray(__facesVAO);
    glBindBuffer(GL_ARRAY_BUFFER, __facesVBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, __vertices.size() * sizeof(VertexNormalTexture), __vertices.data());

    glBindBuffer(GL_ARRAY_BUFFER, 0);
}
