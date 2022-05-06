/*****************************************************************//**
 * \file   Entity.cpp
 * \brief  Entity class source code
 * 
 * \author Kevin Pruvost (pruvostkevin0@gmail.com)
 * \date   March, 28 2022
 *********************************************************************/
#include "Entity.hpp"

// C++ includes
#include <limits>

static std::unique_ptr<Shader> defaultPointShader(nullptr);
static std::unique_ptr<Shader> defaultWireframeShader(nullptr);
static std::unique_ptr<Shader> defaultFaceShader(nullptr);

Entity::Entity(const Mesh & mesh,
    const Shader & pointShader,
    const Shader & wireframeShader,
    const Shader & faceShader,
    const glm::vec3 & defaultPosition,
    const glm::vec3 & defaultEulerAngles,
    const glm::vec3 & defaultScale)
    : Entity_Skeleton()
    , __mesh{ mesh }
    , __shaderPoint{ pointShader }
    , __shaderWireframe{ wireframeShader }
    , __shaderFace{ faceShader }
    , pos{ defaultPosition }
    , scale{ defaultScale }
    , quat{ defaultEulerAngles }
{
}

Entity::Entity(const Mesh & mesh, const glm::vec3 & defaultPosition, const glm::vec3 & defaultEulerAngles, const glm::vec3 & defaultScale)
try : Entity(mesh, *defaultPointShader, *defaultWireframeShader, *defaultFaceShader,
        defaultPosition, defaultEulerAngles, defaultScale)
{
}
catch (const std::exception & e)
{
    LOG_PRINT(stderr, "Cannot load Entity because there's no defaultShaders.\n");
    throw std::runtime_error("Entity Default Shaders");
}

Entity::~Entity()
{
}

void Entity::SetPointShader(const Shader & shader)
{
    __shaderPoint = shader;
}

void Entity::SetWireframeShader(const Shader & shader)
{
    __shaderWireframe = shader;
}

void Entity::SetFaceShader(const Shader & shader)
{
    __shaderFace = shader;
}

Shader & Entity::GetPointShader()
{
    return __shaderPoint;
}

Shader & Entity::GetWireframeShader()
{
    return __shaderWireframe;
}

Shader & Entity::GetFaceShader()
{
    return __shaderFace;
}

void Entity::SetMesh(const Mesh & mesh)
{
    __mesh = Mesh(mesh);
}

void Entity::SetTexture(const Texture & texture)
{
    __texture = texture;
}

const Texture & Entity::GetTexture() const
{
    return __texture;
}

const Mesh & Entity::GetMesh() const { return __mesh; }

glm::mat4 Entity::GetModelMatrix(bool ignoreRotation, bool ignoreScale) const
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

glm::vec3 Entity::GetLocalPosition() const
{
    return pos;
}

void SetDefaultPointShader(const Shader & shader)
{
    defaultPointShader.reset(new Shader(shader.GetShaderDatabaseID()));
}

void SetDefaultWireframeShader(const Shader & shader)
{
    defaultWireframeShader.reset(new Shader(shader.GetShaderDatabaseID()));
}

void SetDefaultFaceShader(const Shader & shader)
{
    defaultFaceShader.reset(new Shader(shader.GetShaderDatabaseID()));
}

const Shader & GetDefaultPointShader()
{
    return *defaultPointShader;
}

const Shader & GetDefaultWireframeShader()
{
    return *defaultWireframeShader;
}

const Shader & GetDefaultFaceShader()
{
    return *defaultFaceShader;
}
