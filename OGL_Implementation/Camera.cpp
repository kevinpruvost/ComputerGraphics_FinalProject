/*****************************************************************//**
 * \file   Camera.cpp
 * \brief  Source code of Camera class
 * 
 * \author Kevin Pruvost (pruvostkevin0@gmail.com)
 * \date   March, 27 2022
 *********************************************************************/
#include "Camera.hpp"

// Project includes
#include "OGL_Implementation\DebugInfo\Log.hpp"
#include "Constants.hpp"

// GLM includes
#include <glm\gtx\vector_angle.hpp>

Camera * mainCamera = nullptr;

Camera::Camera(int windowWidth, int windowHeight,
    GLfloat posX, GLfloat posY, GLfloat posZ, GLfloat upX, GLfloat upY, GLfloat upZ,
    GLfloat yaw, GLfloat pitch)
    : Front{ glm::vec3(0.0f, 0.0f, -1.0f) }
    , MovementSpeed{ CameraDefault_Speed }
    , MouseSensitivity{ CameraDefault_Sensitivity }
    , Zoom{ CameraDefault_Zoom }
    , Position{ glm::vec3(posX, posY, posZ) }
    , WorldUp{ glm::vec3(upX, upY, upZ) }
    , Yaw{ yaw }
    , Pitch{ pitch }
    , __fov{ 45.0f }
    , __zNear{ 0.1f }
    , __zFar{ 10000.0f }
    , __hasMoved{ true }
    , __hasReshaped{ true }
    , __uboCameraProps{ 0 }
    , __uboProjection{ 0 }
    , __wWidth{ windowWidth }
    , __wHeight{ windowHeight }
{
    updateCameraVectors();
    
    // Allocating UBO ViewProj
    glGenBuffers(1, &__uboCameraProps);
    glGenBuffers(1, &__uboProjection);


    glBindBuffer(GL_UNIFORM_BUFFER, __uboCameraProps);
    constexpr const size_t uboCameraPropsSize = sizeof(glm::vec4) + sizeof(glm::mat4) * 3;
    glBufferData(GL_UNIFORM_BUFFER, uboCameraPropsSize, NULL, GL_DYNAMIC_DRAW);
    glBindBufferRange(GL_UNIFORM_BUFFER, Constants::UBO::Ids::cameraProps, __uboCameraProps, 0, uboCameraPropsSize);

    glBindBuffer(GL_UNIFORM_BUFFER, __uboProjection);
    constexpr const size_t uboProjectionSize = sizeof(glm::mat4);
    glBufferData(GL_UNIFORM_BUFFER, uboProjectionSize, NULL, GL_DYNAMIC_DRAW);
    glBindBufferRange(GL_UNIFORM_BUFFER, Constants::UBO::Ids::projection, __uboProjection, 0, uboProjectionSize);
}

Camera::Camera(int windowWidth, int windowHeight, glm::vec3 position, glm::vec3 up, GLfloat yaw, GLfloat pitch)
    : Camera(windowWidth, windowHeight, position.x, position.y, position.z, up.x, up.y, up.z, yaw, pitch)
{
}

Camera::~Camera()
{
    glDeleteBuffers(1, &__uboCameraProps);
    glDeleteBuffers(1, &__uboProjection);

    if (mainCamera == this) mainCamera = nullptr;
}

glm::mat4 Camera::GetViewMatrix()
{
    return __view;
}

glm::mat4 Camera::GetProjectionMatrix()
{
    return __projection;
}

GLuint Camera::GetProjViewMatrixUbo()
{
    if (__hasMoved || __hasReshaped)
    {
        if (__hasMoved)
        {
            __view = glm::lookAt(this->Position, this->Position + this->Front, this->Up);
            __hasMoved = false;
            // Reassign position
            glBindBuffer(GL_UNIFORM_BUFFER, __uboCameraProps);
            glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::vec4), glm::value_ptr(Position));
        }
        if (__hasReshaped)
        {
            __projection = glm::perspective(
                glm::radians(__fov), // FOV
                (GLfloat)__wWidth / (GLfloat)__wHeight, // Aspect Ratio
                __zNear,  // zNear
                __zFar // zFar
            );
            glm::mat4 projection2D = glm::ortho(0.0f, static_cast<GLfloat>(__wWidth), 0.0f, static_cast<GLfloat>(__wHeight), -__zFar, __zFar);
            __hasReshaped = false;

            // Reassign projection matrix
            glBindBuffer(GL_UNIFORM_BUFFER, __uboProjection);
            glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(projection2D));
        }
        // Reassign view & proj matrix
        glBindBuffer(GL_UNIFORM_BUFFER, __uboCameraProps);
        glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::vec4) + 1 * sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(__view));
        glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::vec4) + 2 * sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(__projection));

        // Reassign viewProj matrix
        const glm::mat4 viewProj = __projection * __view;
        glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::vec4), sizeof(glm::mat4), glm::value_ptr(viewProj));
        glBindBuffer(GL_UNIFORM_BUFFER, 0);
    }
    return __uboCameraProps;
}

void Camera::ProcessKeyboard(Camera_Movement direction, GLfloat deltaTime)
{
    __hasMoved = true;
    GLfloat velocity = this->MovementSpeed * deltaTime;
    switch (direction)
    {
        case Camera_Movement::Forward:
            this->Position += this->Front * velocity;
            break;
        case Camera_Movement::Backward:
            this->Position -= this->Front * velocity;
            break;
        case Camera_Movement::Left:
            this->Position -= this->Right * velocity;
            break;
        case Camera_Movement::Right:
            this->Position += this->Right * velocity;
            break;
        case Camera_Movement::Up:
            this->Position += this->Up * velocity;
            break;
        case Camera_Movement::Down:
            this->Position -= this->Up * velocity;
            break;
    }
}

void Camera::ProcessMouseMovement(GLfloat xoffset, GLfloat yoffset, GLboolean constrainPitch)
{
    xoffset *= this->MouseSensitivity;
    yoffset *= this->MouseSensitivity;

    this->Yaw += xoffset;
    this->Pitch += yoffset;

    // Make sure that when pitch is out of bounds, screen doesn't get flipped
    if (constrainPitch)
    {
        if (this->Pitch > 89.0f)
            this->Pitch = 89.0f;
        if (this->Pitch < -89.0f)
            this->Pitch = -89.0f;
    }

    // Update Front, Right and Up Vectors using the updated Eular angles
    this->updateCameraVectors();
}

void Camera::ProcessMouseScroll(GLfloat yoffset)
{
    if (this->Zoom >= 1.0f && this->Zoom <= 45.0f)
        this->Zoom -= yoffset;
    if (this->Zoom <= 1.0f)
        this->Zoom = 1.0f;
    if (this->Zoom >= 45.0f)
        this->Zoom = 45.0f;
    __hasMoved = true;
}

void Camera::Reshape(int windowWidth, int windowHeight, float fov, float zNear, float zFar)
{
    __wWidth = windowWidth;
    __wHeight = windowHeight;
    __fov = fov;
    __zNear = zNear;
    __zFar = zFar;
    __hasReshaped = true;
}

const float & Camera::GetFov() const { return __fov; }
const float & Camera::GetZNear() const { return __zNear; }
const float & Camera::GetZFar() const { return __zFar; }

void Camera::SetFov(float fov)
{
    __fov = fov;
    __hasReshaped = true;
}

void Camera::SetZNearFar(float zNear, float zFar)
{
    __zNear = zNear;
    __zFar = zFar;
    __hasReshaped = true;
}

void Camera::SetWindowDimensions(int windowWidth, int windowHeight)
{
    __wWidth = windowWidth;
    __wHeight = windowHeight;
    __hasReshaped = true;
}

glm::vec2 Camera::GetWindowDimensions() const
{
    return {__wWidth, __wHeight};
}

void Camera::LookAt(const glm::vec3 & objPosition)
{
    __hasMoved = true;
    const glm::mat4 view = glm::lookAt(Position, objPosition, WorldUp);
    const glm::mat4 inverted = glm::inverse(view);
    const glm::vec3 direction = -glm::vec3(inverted[2]);
    Yaw = glm::degrees(glm::atan(direction.z, direction.x));
    Pitch = glm::degrees(glm::asin(direction.y));
    updateCameraVectors();
}

void Camera::updateCameraVectors()
{
    // Calculate the new Front vector
    glm::vec3 front;
    front.x = cos(glm::radians(this->Yaw)) * cos(glm::radians(this->Pitch));
    front.y = sin(glm::radians(this->Pitch));
    front.z = sin(glm::radians(this->Yaw)) * cos(glm::radians(this->Pitch));
    this->Front = glm::normalize(front);
    // Also re-calculate the Right and Up vector
    this->Right = glm::normalize(glm::cross(this->Front, this->WorldUp));  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
    this->Up = glm::normalize(glm::cross(this->Right, this->Front));
    __hasMoved = true;
}