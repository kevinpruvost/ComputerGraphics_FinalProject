/*****************************************************************//**
 * \file   PointLight.hpp
 * \brief  Light class
 * 
 * \author Kevin Pruvost (pruvostkevin0@gmail.com)
 * \date   April, 11 2022
 *********************************************************************/
#pragma once

// Project includes
#include "OGL_Implementation\Entity\Entity.hpp"

/**
 * @brief Structure aligned with the std140 norm that contains
 * parameters of point lights for shaders.
*/
struct PointLight_Shader
{
    glm::vec3 position; // 0
    float constant; // 16

    glm::vec3 ambient; // 32
    float linear; // 20

    glm::vec3 diffuse; // 48
    float quadratic; // 24

    glm::vec3 alignas(16) specular; // 64
};

/**
 * @brief Inherits Entity class and represents Lighting points
*/
class PointLight : public Entity
{
public:
    /**
     * @brief Default constructor
     * @param mesh
     * @param pointShader 
     * @param wireframeShader 
     * @param lightShader 
     * @param defaultPosition
     * @param defaultEulerAngles
     * @param defaultScale
     * @param mesh
     * @param defaultPosition
     * @param defaultEulerAngles
     * @param defaultScale
     * @param constant
     * @param linear
     * @param quadratic
     * @param ambient
     * @param diffuse
     * @param specular
    */
    PointLight(const Mesh & mesh,
        const Shader & pointShader,
        const Shader & wireframeShader,
        const Shader & lightShader,
        const glm::vec3 & defaultPosition = glm::vec3(0.0f, 0.0f, 0.0f),
        const glm::vec3 & defaultEulerAngles = glm::vec3(0.0f, 0.0f, 0.0f),
        const glm::vec3 & defaultScale = glm::vec3(1.0f, 1.0f, 1.0f),
        const float constant = 1.0f, const float linear = 0.09f, const float quadratic = 0.032f,
        const glm::vec3 & ambient = glm::vec3(0.05f),
        const glm::vec3 & diffuse = glm::vec3(0.8f),
        const glm::vec3 & specular = glm::vec3(1.0f));

    /**
     * @brief Constructor with default point/wireframe/face shader already set
     * /!\ I repeat, do not forget to set the appropriate default shaders
     * @param mesh
     * @param defaultPosition
     * @param defaultEulerAngles
     * @param defaultScale
     * @param mesh 
     * @param defaultPosition 
     * @param defaultEulerAngles 
     * @param defaultScale 
     * @param constant 
     * @param linear 
     * @param quadratic 
     * @param ambient 
     * @param diffuse 
     * @param specular 
    */
    PointLight(const Mesh & mesh,
        const glm::vec3 & defaultPosition = glm::vec3(0.0f, 0.0f, 0.0f),
        const glm::vec3 & defaultEulerAngles = glm::vec3(0.0f, 0.0f, 0.0f),
        const glm::vec3 & defaultScale = glm::vec3(1.0f, 1.0f, 1.0f),
        const float constant = 1.0f, const float linear = 0.09f, const float quadratic = 0.032f,
        const glm::vec3 & ambient = glm::vec3(0.05f),
        const glm::vec3 & diffuse = glm::vec3(0.8f),
        const glm::vec3 & specular = glm::vec3(1.0f));
    ~PointLight();

    /**
     * @brief Maximum count of Point Lights in a scene.
    */
    static constexpr const size_t maxPointLightsCount = 128;

    virtual glm::mat4 GetModelMatrix(bool ignoreRotation = false, bool ignoreScale = false) const override;

    PointLight_Shader GetShaderInfo() const;

    void ChangeBrightnessSettings(const float constant, const float linear, const float quadratic);
    void ChangeAmbient(const glm::vec3 & ambient);
    void ChangeDiffuse(const glm::vec3 & diffuse);
    void ChangeSpecular(const glm::vec3 & specular);

public:
    static const size_t GetPointLightsCount();
    static const std::array<PointLight *, 128> & GetAllPointLights();

private:
    void InsertPointLight();
    void DeletePointLight();

public:
    float __constant;
    float __linear;
    float __quadratic;

    glm::vec3 __ambient;
    glm::vec3 __diffuse;
    glm::vec3 __specular;
};

void SetDefaultLightShader(const Shader & shader);
const Shader & GetDefaultLightShader();
