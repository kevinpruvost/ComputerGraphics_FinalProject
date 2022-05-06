/*****************************************************************//**
 * \file   AxisDisplayer.hpp
 * \brief  AxisDisplayer class
 * 
 * \author Kevin Pruvost (pruvostkevin0@gmail.com)
 * \date   May, 03 2022
 *********************************************************************/
#pragma once

// Project includes
#include "OGL_Implementation\Camera.hpp"
#include "OGL_Implementation\Quaternion.hpp"
#include "OGL_Implementation\Shader\Shader.hpp"

/**
 * @brief Displays all 3 axis (x,y,z) in red,green,blue respectively
*/
class AxisDisplayer
{
public:
    /**
     * @brief Constructs Axis Displayer with its shader and the sizes of each axis
     * @param shader_ 
     * @param axisSize 
    */
    AxisDisplayer(const Shader & shader_, const GLfloat axisSize = 60.0f);
    ~AxisDisplayer();

public:
    void Draw();
    glm::mat4 GetModelMatrix() const;

public:
    glm::vec2 pos;
    Quaternion quat;

    Shader shader;

private:
    GLuint __xVAO, __yVAO, __zVAO;
    GLuint __xVBO, __yVBO, __zVBO;
};