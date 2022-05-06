/*****************************************************************//**
 * \file   Text.cpp
 * \brief  Text source code
 * 
 * \author Kevin Pruvost (pruvostkevin0@gmail.com)
 * \date   April, 06 2022
 *********************************************************************/
#include "Text.hpp"

// C++ includes
#include <memory>

static std::unique_ptr<Font> defaultFont(nullptr);
static std::unique_ptr<Shader> defaultText2DShader(nullptr);
static std::unique_ptr<Shader> defaultText3DShader(nullptr);

Text2D::Text2D(const Font & _font, const Shader & _shader, const std::string & _str, const glm::vec2 & xy, const GLfloat _scale, const glm::vec3 & _color)
    : str{ _str }
    , pos{ xy }
    , scale{ _scale }
    , color{ _color }
    , font{ _font }
    , shader{ _shader }
    , centered{ true }
{
}

Text2D::Text2D(const std::string & _str, const glm::vec2 & xy, const GLfloat _scale, const glm::vec3 & _color)
try : Text2D(*defaultFont, *defaultText2DShader, _str, xy, _scale, _color)
{
}
catch (const std::exception & e)
{
    LOG_PRINT(stderr, "Cannot load Text2D because there's no defaultFont or defaultText2DShader: %s\n", e.what());
    throw std::runtime_error("Text2D Default Font/Shader");
}

Text3D::Text3D(const Font & _font, const Shader & _shader, const std::string & _str, const glm::vec3 & xyz, const GLfloat _scale, const glm::vec3 & _color)
    : str{ _str }
    , pos{ xyz }
    , scale{ _scale }
    , color{ _color }
    , font{ _font }
    , shader{ _shader }
    , centered{ true }
{
}

Text3D::Text3D(const std::string & _str, const glm::vec3 & xyz, const GLfloat _scale, const glm::vec3 & _color)
try : Text3D(*defaultFont, *defaultText3DShader, _str, xyz, _scale, _color)
{
} catch (const std::exception & e)
{
    LOG_PRINT(stderr, "Cannot load Text3D because there's no defaultFont or defaultText3DShader: %s\n", e.what());
    throw std::runtime_error("Text3D Default Font/Shader");
}

glm::mat4 Text3D::GetModelMatrix(bool ignoreRotation, bool ignoreScale) const
{
    // Transformation Matrix
    glm::mat4 mat = {
        {1.0f, 0.0f, 0.0f, 0.0f},
        {0.0f, 1.0f, 0.0f, 0.0f},
        {0.0f, 0.0f, 1.0f, 0.0f},
        {pos.x, pos.y, pos.z, 1.0f}
    };
    // Scaling Matrix
    if (!ignoreScale)
        mat = glm::scale(mat, glm::vec3(1.0f));
    if (HasParent())
        mat = this->GetParent()->GetModelMatrix(true, false) * mat;
    return mat;
}

glm::vec3 Text3D::GetLocalPosition() const
{
    return pos;
}

void SetDefaultFont(const Font & font)
{
    defaultFont.reset(new Font(font.GetFontDatabaseID()));
}

void SetDefault2DTextShader(const Shader & shader)
{
    defaultText2DShader.reset(new Shader(shader.GetShaderDatabaseID()));
}

void SetDefault3DTextShader(const Shader & shader)
{
    defaultText3DShader.reset(new Shader(shader.GetShaderDatabaseID()));
}