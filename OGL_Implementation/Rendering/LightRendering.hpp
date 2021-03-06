/*****************************************************************//**
 * \file   Light.hpp
 * \brief  Light Main Header
 * 
 * \author Kevin Pruvost (pruvostkevin0@gmail.com)
 * \date   April, 11 2022
 *********************************************************************/
#pragma once

// Project includes
#include "OGL_Implementation\Light\Light.hpp"

/**
 * @brief Manages All variables needed GPU-side and gives everything for shaders
 * to work with lights.
*/
class LightRendering
{
public:
    LightRendering();
    ~LightRendering();

    GLuint GetUboLights();
    Shader & GetShadowMappingShader();
    const std::array<GLuint, PointLight::maxPointLightsCount> & GetDepthMapFbo() const;

    static void Init();
    static const LightRendering & Get();
    static void RefreshUbo();

    static void PrintDepthMap();

public:
    std::array<GLuint, PointLight::maxPointLightsCount> shadowMaps;
    bool screenshotDepthMap;

private:
    std::array<GLuint, PointLight::maxPointLightsCount> __depthMapFbo;
    GLuint __uboLights;
    Shader __shadowMappingShader;
};

/**
 * @brief TODO
*/
struct SpotLight_Shader
{
    glm::vec3 position; // 0
    glm::vec3 direction; // 16
    float cutOff; // 32
    float outerCutOff; // 36

    float constant; // 40
    float linear; // 44
    float quadratic; // 48

    glm::vec3 ambient; // 64
    glm::vec3 diffuse; // 80
    glm::vec3 specular; // 96
};

/**
 * @brief TODO
*/
struct DirectionLight_Shader
{
    glm::vec3 direction;

    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
};