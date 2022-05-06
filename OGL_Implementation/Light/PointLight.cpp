/*****************************************************************//**
 * \file   PointLight.cpp
 * \brief  PointLight source code
 * \author Kevin Pruvost (pruvostkevin0@gmail.com)
 * \date   April, 11 2022
 *********************************************************************/
#include "PointLight.hpp"

static std::unique_ptr<Shader> defaultLightShader(nullptr);

static std::array<PointLight *, PointLight::maxPointLightsCount> pointLights = { nullptr };
static size_t maxPos = 0;

PointLight::PointLight(const Mesh & mesh, const Shader & pointShader, const Shader & wireframeShader, const Shader & lightShader, const glm::vec3 & defaultPosition, const glm::vec3 & defaultEulerAngles, const glm::vec3 & defaultScale,
    const float constant, const float linear, const float quadratic, const glm::vec3 & ambient, const glm::vec3 & diffuse, const glm::vec3 & specular)
    : Entity(mesh, pointShader, wireframeShader, lightShader, defaultPosition, defaultEulerAngles, defaultScale)
    , __constant{ constant }
    , __linear{ linear }
    , __quadratic{ quadratic }
    , __ambient{ ambient }
    , __diffuse{ diffuse }
    , __specular{ specular }
{
    InsertPointLight();
}

PointLight::PointLight(const Mesh & mesh, const glm::vec3 & defaultPosition, const glm::vec3 & defaultEulerAngles, const glm::vec3 & defaultScale,
    const float constant, const float linear, const float quadratic, const glm::vec3 & ambient, const glm::vec3 & diffuse, const glm::vec3 & specular)
    : PointLight(mesh, GetDefaultPointShader(), GetDefaultWireframeShader(), GetDefaultLightShader(), defaultPosition, defaultEulerAngles, defaultScale,
        constant, linear, quadratic, ambient, diffuse, specular)
{
}

PointLight::~PointLight()
{
    DeletePointLight();
}

glm::mat4 PointLight::GetModelMatrix(bool ignoreRotation, bool ignoreScale) const
{
    // Transformation Matrix
    glm::mat4 mat = {
        {1.0f, 0.0f, 0.0f, 0.0f},
        {0.0f, 1.0f, 0.0f, 0.0f},
        {0.0f, 0.0f, 1.0f, 0.0f},
        {pos.x, pos.y, pos.z, 1.0f}
    };
    // Rotation Matrix
    if (!ignoreRotation)
        mat *= glm::toMat4(quat);
    // Scaling Matrix
    if (!ignoreScale)
        mat = glm::scale(mat, scale);

    if (HasParent())
        mat = this->GetParent()->GetModelMatrix(false, true) * mat;
    return mat;
}

PointLight_Shader PointLight::GetShaderInfo() const
{
    PointLight_Shader shaderInfo{
        pos,
        __constant,
        __ambient,
        __linear,
        __diffuse,
        __quadratic,
        __specular
    };
    return shaderInfo;
}

void PointLight::ChangeBrightnessSettings(const float constant, const float linear, const float quadratic)
{
    __constant = constant;
    __linear = linear;
    __quadratic = quadratic;
}

void PointLight::ChangeAmbient(const glm::vec3 & ambient) { __ambient = ambient; }
void PointLight::ChangeDiffuse(const glm::vec3 & diffuse) { __diffuse = diffuse; }
void PointLight::ChangeSpecular(const glm::vec3 & specular) { __specular = specular; }

const size_t PointLight::GetPointLightsCount()
{
    return maxPos;
}

const std::array<PointLight *, 128> & PointLight::GetAllPointLights()
{
    return pointLights;
}

void PointLight::InsertPointLight()
{
    if (maxPos >= PointLight::maxPointLightsCount) throw std::runtime_error("Too much PointLight instances.");
    pointLights[maxPos++] = this;
}

void PointLight::DeletePointLight()
{
    for (size_t i = 0; i < pointLights.size(); ++i)
    {
        if (pointLights[i] == this)
        {
            while (i + 1 < pointLights.size())
            {
                pointLights[i] = pointLights[i + 1];
                ++i;
            }
        }
    }
    --maxPos;
}

void SetDefaultLightShader(const Shader & shader)
{
    defaultLightShader.reset(new Shader(shader.GetShaderDatabaseID()));
}

const Shader & GetDefaultLightShader()
{
    return *defaultLightShader;
}
