/*****************************************************************//**
 * \file   AxisDisplayer.cpp
 * \brief  3D AxisDisplayer source code
 * 
 * \author Kevin Pruvost (pruvostkevin0@gmail.com)
 * \date   May, 03 2022
 *********************************************************************/
#include "AxisDisplayer.hpp"

// Project includes
#include "OGL_Implementation\Window.hpp"

AxisDisplayer::AxisDisplayer(const Shader & shader_, const GLfloat axisSize)
    : pos{ 0.07f, 0.1f }
    , shader{ shader_ }
{
    glGenVertexArrays(3, &__xVAO);
    glGenBuffers(3, &__xVBO);

    {
        const GLfloat vertices[] = {
            0.0f, 0.0f, 0.0f,
            axisSize, 0.0f, 0.0f
        };
        glBindVertexArray(__xVAO);
        // Fill mesh buffer
        glBindBuffer(GL_ARRAY_BUFFER, __xVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        // Set mesh attributes
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);
    }
    {
        const GLfloat vertices[] = {
            0.0f, 0.0f, 0.0f,
            0.0f, axisSize, 0.0f
        };
        glBindVertexArray(__yVAO);
        // Fill mesh buffer
        glBindBuffer(GL_ARRAY_BUFFER, __yVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        // Set mesh attributes
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);
    }
    {
        const GLfloat vertices[] = {
            0.0f, 0.0f, 0.0f,
            0.0f, 0.0f, axisSize
        };
        glBindVertexArray(__zVAO);
        // Fill mesh buffer
        glBindBuffer(GL_ARRAY_BUFFER, __zVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        // Set mesh attributes
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);
    }
}

AxisDisplayer::~AxisDisplayer()
{
    glDeleteVertexArrays(3, &__xVAO);
    glDeleteBuffers(3, &__xVBO);
}

void AxisDisplayer::Draw()
{
    //quat = Quaternion({ 0.0f, -mainCamera->Yaw, -mainCamera->Pitch });

    glm::mat4 model = GetModelMatrix();
    glm::mat4 viewMatrix = glm::lookAt(glm::vec3{ 0.0f }, mainCamera->Front, mainCamera->Up );
    glm::mat4 newModel = viewMatrix * model;
    // Removes position modification from viewMatrix * model
    newModel[3][0] = model[3][0];
    newModel[3][1] = model[3][1];
    newModel[3][2] = model[3][2];

    shader.Use();

    shader.SetUniformMatrix4f("model", newModel);

    glLineWidth(3.0f);

    glBindVertexArray(__xVAO);
    glDrawArrays(GL_LINES, 0, 2);
    glBindVertexArray(__yVAO);
    glDrawArrays(GL_LINES, 0, 2);
    glBindVertexArray(__zVAO);
    glDrawArrays(GL_LINES, 0, 2);
    glBindVertexArray(0);

    glLineWidth(1.0f);
}

glm::mat4 AxisDisplayer::GetModelMatrix() const
{
    // Transformation Matrix
    glm::mat4 mat = {
        {1.0f, 0.0f, 0.0f, 0.0f},
        {0.0f, 1.0f, 0.0f, 0.0f},
        {0.0f, 0.0f, 1.0f, 0.0f},
        {pos.x * Window::Get()->windowWidth(), pos.y * Window::Get()->windowHeight(), 0.0f, 1.0f}
    };
    // Rotation Matrix
    mat *= glm::toMat4(quat);
    return mat;
}