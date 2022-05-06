/*****************************************************************//**
 * \file   Light.cpp
 * \brief  Light source code
 * 
 * \author Kevin Pruvost (pruvostkevin0@gmail.com)
 * \date   April, 12 2022
 *********************************************************************/
#include "LightRendering.hpp"

// Project includes
#include "Constants.hpp"

static std::unique_ptr<LightRendering> s_lightRendering;

LightRendering::LightRendering()
    : __uboLights{ 0 }
{
    // Allocating UBO ViewProj
    glGenBuffers(1, &__uboLights);

    constexpr const size_t LightsSize = sizeof(PointLight_Shader) * PointLight::maxPointLightsCount
//      + sizeof(DirectionLight_Shader)
//      + sizeof(SpotLight_Shader)
      + sizeof(int);
    glBindBuffer(GL_UNIFORM_BUFFER, __uboLights);
    glBufferData(GL_UNIFORM_BUFFER, LightsSize, NULL, GL_DYNAMIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    // Binds buffer to a specific binding point so that it'll be used at this exact place
    // by shaders
    glBindBufferRange(GL_UNIFORM_BUFFER, Constants::UBO::Ids::lights, __uboLights, 0, LightsSize);
}

LightRendering::~LightRendering()
{
    glDeleteBuffers(1, &__uboLights);
}

GLuint LightRendering::GetUboLights()
{
    return __uboLights;
}

void LightRendering::Init()
{
    s_lightRendering.reset(new LightRendering());
}

const LightRendering & LightRendering::Get()
{
    if (s_lightRendering) return *s_lightRendering;
    throw std::runtime_error("Trying to get LightRendering although it has not been initialized.\n");
}

void LightRendering::RefreshUbo()
{
    const int pointLightsCount = PointLight::GetPointLightsCount();
    const auto & pointLights = PointLight::GetAllPointLights();

    glBindBuffer(GL_UNIFORM_BUFFER, s_lightRendering->GetUboLights());

    for (size_t i = 0; i < pointLightsCount; ++i)
    {
//        if (pointLights[i]->HasChanged())
//        {
        auto shaderInfo = pointLights[i]->GetShaderInfo();
        glBufferSubData(GL_UNIFORM_BUFFER, i * sizeof(PointLight_Shader), sizeof(PointLight_Shader), &shaderInfo);
//        }
    }
    glBufferSubData(GL_UNIFORM_BUFFER, sizeof(PointLight_Shader) * PointLight::maxPointLightsCount,
        sizeof(int), &pointLightsCount);
}
