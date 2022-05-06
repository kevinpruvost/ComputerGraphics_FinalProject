/*****************************************************************//**
 * \file   Input.hpp
 * \brief  Managing GLFW input
 * 
 * \author Kevin Pruvost (pruvostkevin0@gmail.com)
 * \date   March, 26 2022
 *********************************************************************/
#pragma once

// C++ includes
#include <iostream>
#include <functional>

// GLAD
#include <glad/glad.h>

// GLFW
#include <GLFW/glfw3.h>

// GLM Mathematics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

/**
 * @brief Defines Input Key State.
*/
enum class InputKey : unsigned char
{
    Released     = 0, // Released (Default State)
    JustPressed  = 1, // Just pressed and about to go InputKey::Pressed
    Pressed      = 2, // Maintaining pressure
    JustReleased = 3, // Just released and about to go InputKey::Released
    InputKeyError = 4
};

// Callbacks used by GLFW
void InputKeyCallback(GLFWwindow * window, int key, int scancode, int action, int mode);
void InputCallbackCorrection();

void InputMouseCallback(GLFWwindow * window, double xpos, double ypos);
void InputScrollCallback(GLFWwindow * window, double xoffset, double yoffset);

/**
 * @brief Manages Input from mouse and keyboard.
*/
class Input
{
public:
    /**
     * @brief Initialize the first important input states.
     * Call to Init() is mandatory before launching the event loop.
     * @param window
    */
    void Init(GLFWwindow * window);

    /**
     * @brief Returns Key State.
     * @param index 
     * @return key state if index contained 0 < index < 1024, else InputKey::Released
    */
    InputKey key(unsigned int index) const;
    /**
     * @brief Returns mouse position in a 2D vector.
     * @return mouse position
    */
    glm::dvec2 mousePosition() const;
    /**
     * @brief Returns mouse offset from last position.
     * @return mouse offset
    */
    glm::vec2 mousePositionOffset() const;
    /**
     * @brief Returns mouse wheel offset.
     * @return mouse wheel offset
    */
    glm::vec2 mouseWheelOffset() const;
    /**
     * @brief Returns if the mouse has moved or not during the last frame.
     * @return has moved
    */
    bool mouseHasMoved() const;
    /**
     * @brief Returns if the mouse wheel has moved or not during the last frame.
     * @return has moved
    */
    bool mouseScrollHasMoved() const;

protected:
};
