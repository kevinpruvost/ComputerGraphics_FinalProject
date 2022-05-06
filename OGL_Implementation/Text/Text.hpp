/*****************************************************************//**
 * \file   Text.hpp
 * \brief  Text class
 * 
 * \author Kevin Pruvost (pruvostkevin0@gmail.com)
 * \date   April, 06 2022
 *********************************************************************/
#pragma once

// GLM includes
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>
#include <glm\gtx\euler_angles.hpp>
#include <glm\gtx\quaternion.hpp>

// Project includes
#include "Font.hpp"
#include "OGL_Implementation\Shader\Shader.hpp"
#include "OGL_Implementation\Entity\Entity_Skeleton.hpp"

// C++ includes
#include <string>

/**
 * @brief 2D Text class
*/
class Text2D
{
public:
    /**
     * @brief Default constructor
     * @param Font
     * @param Shader
     * @param String to display
     * @param Position (x, y) between 0.0f and 1.0f (screen size related)
     * @param Scale
     * @param Color (float, float, float)
    */
    Text2D(const Font & _font, const Shader & _shader, const std::string & _str, const glm::vec2 & xy, const GLfloat _scale, const glm::vec3 & _color);
    /**
     * @brief Shortened constructor (you need to set the default font & shader manually)
     * @param String to display
     * @param Position (x, y) between 0.0f and 1.0f (screen size related)
     * @param Scale
     * @param Color (float, float, float)
    */
    Text2D(const std::string & _str, const glm::vec2 & xy, const GLfloat _scale, const glm::vec3 & _color);

public:
    Font font;
    Shader shader;
    std::string str;
    glm::vec2 pos;
    GLfloat scale;
    glm::vec3 color;
    bool centered;
};

/**
 * @brief 3D Text class
*/
class Text3D : public Entity_Skeleton
{
public:
    /**
     * @brief Default constructor
     * @param Font
     * @param Shader
     * @param String to display
     * @param Position (x, y, z)
     * @param Scale
     * @param Color (float, float, float)
    */
    Text3D(const Font & _font, const Shader & _shader, const std::string & _str, const glm::vec3 & xyz, const GLfloat _scale, const glm::vec3 & _color);
    /**
     * @brief Shortened constructor (you need to set the default font & shader manually)
     * @param String to display
     * @param Position (x, y, z)
     * @param Scale
     * @param Color (float, float, float)
    */
    Text3D(const std::string & _str, const glm::vec3 & xyz, const GLfloat _scale, const glm::vec3 & _color);

    /**
     * @brief Calculates Model Matrix.
     * @return Model Matrix
    */
    virtual glm::mat4 GetModelMatrix(bool ignoreRotation = false, bool ignoreScale = false) const;

    // Entity_Skeleton abstract
    virtual glm::vec3 GetLocalPosition() const;

public:
    Font font;
    Shader shader;
    std::string str;
    glm::vec3 pos;
    GLfloat scale;
    glm::vec3 color;
    bool centered;
};

void SetDefaultFont(const Font & font);
void SetDefault2DTextShader(const Shader & shader);
void SetDefault3DTextShader(const Shader & shader);