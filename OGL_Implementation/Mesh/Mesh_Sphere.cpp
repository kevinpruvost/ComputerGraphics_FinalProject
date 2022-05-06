/*****************************************************************//**
 * \file   Mesh_Sphere.cpp
 * \brief  Mesh_Sphere Source code
 * 
 * \author Kevin Pruvost (pruvostkevin0@gmail.com)
 * \date   April, 03 2022
 *********************************************************************/
#include "Mesh_Sphere.hpp"

// Project includes
#include "OGL_Implementation\DebugInfo\Log.hpp"

// GLM includes
#include <glm/gtx/normal.hpp>

// C++ includes
#include <vector>

Mesh_Sphere::Mesh_Sphere(float radius, int sectors, int stacks, bool smooth)
    : __radius{ radius }
    , __sectors{ sectors }
    , __stacks{ stacks }
    , __smooth{ smooth }
    , __facesEBO{ 0 }
{
    LOG_PRINT(Log::LogMainFileName, "Constructed\n");

    glGenBuffers(1, &__facesEBO);

    if (__smooth)
        buildVerticesSmooth();
    else
        buildVerticesFlat();
}

Mesh_Sphere::~Mesh_Sphere()
{
    LOG_PRINT(Log::LogMainFileName, "Destroyed\n");

    glDeleteBuffers(1, &__facesEBO);
}

GLuint Mesh_Sphere::GetFacesEBO() const
{
    return __facesEBO;
}

bool Mesh_Sphere::IsUsingEBO() const
{
    return true;
}

void Mesh_Sphere::buildVertices(std::vector<Vertex> & vertices, std::vector<VertexNormalTexture> & vnts, bool calculateNormal)
{
    vertices.reserve(static_cast<size_t>(__stacks) * static_cast<size_t>(__sectors));
    vnts.reserve(static_cast<size_t>(__stacks) * static_cast<size_t>(__sectors));

    Vertex vertex;
    VertexNormalTexture vnt;
    float xy;
    float lengthInv = 1.0f / __radius; // normal

    float sectorStep = 2 * Pi / __sectors;
    float stackStep = Pi / __stacks;
    float sectorAngle, stackAngle;

    for (int i = 0; i <= __stacks; ++i)
    {
        stackAngle = Pi / 2 - i * stackStep;          // starting from pi/2 to -pi/2
        xy = __radius * cosf(stackAngle);             // r * cos(u)
        vertex.z = vnt.z = __radius * sinf(stackAngle);              // r * sin(u)

        // add (__sectors+1) vertices per stack
        // the first and last vertices have same position and normal, but different tex coords
        for (int j = 0; j <= __sectors; ++j)
        {
            sectorAngle = j * sectorStep;           // starting from 0 to 2pi

            // vertex position
            vertex.x = vnt.x = xy * cosf(sectorAngle);             // r * cos(u) * cos(v)
            vertex.y = vnt.y = xy * sinf(sectorAngle);             // r * cos(u) * sin(v)

            // vertex tex coord between [0, 1]
            vnt.s = (float)j / __sectors;
            vnt.t = (float)i / __stacks;

            // normalized vertex normal
            if (calculateNormal)
            {
                vnt.nx = vnt.x * lengthInv;
                vnt.ny = vnt.y * lengthInv;
                vnt.nz = vnt.z * lengthInv;
            }

            vertices.push_back(vertex);
            vnts.push_back(vnt);
        }
    }
}

void Mesh_Sphere::bind3SizedVertices(const std::vector<Vertex> & vertices)
{
    // Binding vertices
    glBindVertexArray(__verticesVAO);
    glBindBuffer(GL_ARRAY_BUFFER, __verticesVBO);

    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices.size(), vertices.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid *)0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    __verticesNVert = vertices.size();
}

void Mesh_Sphere::bindVnts(const std::vector<VertexNormalTexture> & vnts, const std::vector<GLuint> & indices)
{
    // Binding vertices with normal & texture coordinates
    glBindVertexArray(__facesVAO);

    glBindBuffer(GL_ARRAY_BUFFER, __facesVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(VertexNormalTexture) * vnts.size(), vnts.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, __facesEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * indices.size(), indices.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid *)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid *)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid *)(6 * sizeof(GLfloat)));

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    __facesNVert = indices.size();
}

void Mesh_Sphere::buildVerticesSmooth()
{
    std::vector<Vertex> vertices;
    std::vector<VertexNormalTexture> vnts;
    buildVertices(vertices, vnts, true);

    bind3SizedVertices(vertices);

    // indices
    //  k1--k1+1
    //  |  / |
    //  | /  |
    //  k2--k2+1
    std::vector<GLuint> indices;
    const size_t count = static_cast<size_t>(__sectors) * (5 + (static_cast<size_t>(__stacks) - 2) * 10 + 7);
    indices.reserve(count);
    unsigned int k1, k2;
    for (int i = 0; i < __stacks; ++i)
    {
        k1 = i * (__sectors + 1);     // beginning of current stack
        k2 = k1 + __sectors + 1;      // beginning of next stack

        for (int j = 0; j < __sectors; ++j, ++k1, ++k2)
        {
            // 2 triangles per sector excluding 1st and last stacks
            if (i != 0)
                indices.insert(indices.end(), { k1, k2, k1 + 1 });

            if (i != (__stacks - 1))
                indices.insert(indices.end(), { k1 + 1, k2, k2 + 1 });

            // vertical lines for all stacks
            //indices.insert(indices.end(), { k1, k2 });
            //if (i != 0)  // horizontal lines except 1st stack
            //    indices.insert(indices.end(), { k1, k1 + 1 });
        }
    }

    bindVnts(vnts, indices);
}

void Mesh_Sphere::buildVerticesFlat()
{
    std::vector<Vertex> vertices;
    std::vector<VertexNormalTexture> vnts;
    buildVertices(vertices, vnts, false);
    bind3SizedVertices(vertices);

    std::vector<GLuint> indices;
    const size_t count = static_cast<size_t>(__sectors) * (5 + (static_cast<size_t>(__stacks) - 2) * 10 + 7);
    indices.reserve(count);

    GLuint v1, v2, v3, v4;   // 4 vertex positions and tex coords
    std::vector<float> n;    // 1 face normal

    int i, j, k;
    int index = 0;                                  // index for vertex
    for (i = 0; i < __stacks; ++i)
    {
        v1 = i * (__sectors + 1);                // index of tmpVertices
        v2 = (i + 1) * (__sectors + 1);

        for (j = 0; j < __sectors; ++j, ++v1, ++v2)
        {
            // get 4 vertices per sector
            //  v1--v3
            //  |    |
            //  v2--v4
            v3 = v1 + 1;
            v4 = v2 + 1;

            // if 1st stack and last stack, store only 1 triangle per sector
            // otherwise, store 2 triangles (quad) per sector
            if (i == 0) // a triangle for first stack ==========================
            {
                // put a triangle
                // addVertex(v1.x, v1.y, v1.z);
                // addVertex(v2.x, v2.y, v2.z);
                // addVertex(v4.x, v4.y, v4.z);

                // put tex coords of triangle
                // addTexCoord(v1.s, v1.t);
                // addTexCoord(v2.s, v2.t);
                // addTexCoord(v4.s, v4.t);

                // put normal
                //n = computeFaceNormal(v1.x, v1.y, v1.z, v2.x, v2.y, v2.z, v4.x, v4.y, v4.z);
                //for (k = 0; k < 3; ++k)  // same normals for 3 vertices
                //{
                //    addNormal(n[0], n[1], n[2]);
                //}
                glm::vec3 normal = glm::triangleNormal(vnts[v1].xyz, vnts[v2].xyz, vnts[v4].xyz);
                for (const auto v : { v1, v2, v4 })
                    vnts[v].normals = normal;

                // put indices of 1 triangle
                //addIndices(index, index + 1, index + 2);
                indices.insert(indices.end(), {v1, v2, v4});

                // indices for line (first stack requires only vertical line)
                //lineIndices.push_back(index);
                //lineIndices.push_back(index + 1);
                //**indices.insert(indices.end(), { v1, v2 });

                index += 3;     // for next
            }
            else if (i == (__stacks - 1)) // a triangle for last stack =========
            {
                // put a triangle
                // addVertex(v1.x, v1.y, v1.z);
                // addVertex(v2.x, v2.y, v2.z);
                // addVertex(v3.x, v3.y, v3.z);

                // put tex coords of triangle
                // addTexCoord(v1.s, v1.t);
                // addTexCoord(v2.s, v2.t);
                // addTexCoord(v3.s, v3.t);

                // put normal
                // n = computeFaceNormal(v1.x, v1.y, v1.z, v2.x, v2.y, v2.z, v3.x, v3.y, v3.z);
                // for (k = 0; k < 3; ++k)  // same normals for 3 vertices
                // {
                //     addNormal(n[0], n[1], n[2]);
                // }
                glm::vec3 normal = glm::triangleNormal(vnts[v1].xyz, vnts[v2].xyz, vnts[v3].xyz);
                for (const auto v : { v1, v2, v3 })
                    vnts[v].normals = normal;

                // put indices of 1 triangle
                // addIndices(index, index + 1, index + 2);
                indices.insert(indices.end(), { v1, v2, v3 });

                // indices for lines (last stack requires both vert/hori lines)
                // lineIndices.push_back(index);
                // lineIndices.push_back(index + 1);
                // lineIndices.push_back(index);
                // lineIndices.push_back(index + 2);
                
                //**indices.insert(indices.end(), { v1, v2, v1, v3 });


                index += 3;     // for next
            }
            else // 2 triangles for others ====================================
            {
                // put quad vertices: v1-v2-v3-v4
                // addVertex(v1.x, v1.y, v1.z);
                // addVertex(v2.x, v2.y, v2.z);
                // addVertex(v3.x, v3.y, v3.z);
                // addVertex(v4.x, v4.y, v4.z);

                // put tex coords of quad
                // addTexCoord(v1.s, v1.t);
                // addTexCoord(v2.s, v2.t);
                // addTexCoord(v3.s, v3.t);
                // addTexCoord(v4.s, v4.t);

                // put normal
                // n = computeFaceNormal(v1.x, v1.y, v1.z, v2.x, v2.y, v2.z, v3.x, v3.y, v3.z);
                // for (k = 0; k < 4; ++k)  // same normals for 4 vertices
                // {
                //     addNormal(n[0], n[1], n[2]);
                // }
                glm::vec3 normal = glm::triangleNormal(vnts[v1].xyz, vnts[v2].xyz, vnts[v3].xyz);
                for (const auto v : { v1, v2, v3, v4 })
                    vnts[v].normals = normal;

                // put indices of quad (2 triangles)
                // addIndices(index, index + 1, index + 2);
                // addIndices(index + 2, index + 1, index + 3);
                indices.insert(indices.end(), {
                    v1, v2, v3,
                    v3, v2, v4
                });

                // indices for lines
                // lineIndices.push_back(index);
                // lineIndices.push_back(index + 1);
                // lineIndices.push_back(index);
                // lineIndices.push_back(index + 2);
                //**indices.insert(indices.end(), { v1, v2, v1, v3 });

                index += 4;     // for next
            }
        }
    }

    bindVnts(vnts, indices);
}

Mesh_Base::DrawMode Mesh_Sphere::GetDrawMode() const
{
    return DrawMode::DrawElements;
}
