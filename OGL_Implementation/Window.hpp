/*****************************************************************//**
 * \file   Window.hpp
 * \brief  Managing OpenGL Window
 * 
 * \author Kevin Pruvost (pruvostkevin0@gmail.com)
 * \date   March, 26 2022
 *********************************************************************/
#pragma once

// Project includes
#include "Input.hpp"

// C++ includes
#include <vector>

/**
 * @brief Manages everything about the OpenGL window.
*/
class Window : public Input
{
public:
    Window();
    ~Window();

    /**
    * @brief Initializes OpenGL window.
    * @param windowName
    * @return true if initialization has succeeded, false otherwise
    */
    bool Initialize(const char * windowName, const char * iconPath = nullptr);
    /**
     * @brief Launches the window loop and the code contained in the lambda function.
     * @param lambda
     * @return lambda's return value
    */
    bool Loop(const std::function<bool()> & lambda);
    /**
     * @brief Calculates period between the current frame & the last frame
     * in seconds
     * @return Delta Time from last frame * delta time multiplier
    */
    float DeltaTime() const;
    /**
     * @brief Calculates period between the current frame & the last frame
     * in seconds
     * @return Delta Time from last frame
    */
    float DeltaTimeNoMultiplier() const;
    /**
     * @brief Returns Window current Width
     * @return window width 
    */
    int windowWidth() const;
    /**
     * @brief Returns Window current Height
     * @return window height
    */
    int windowHeight() const;
    /**
     * @brief window dimensions have changed
     * @return has changed
    */
    bool windowDimensionsHasChanged() const;
    /**
     * @brief Returns if window is focused or not
     * @return window focused 
    */
    bool GetWindowFocused() const;
    /**
     * @brief Sets a delta time multiplier
     * @param newMult 
    */
    void SetTimeMultiplier(const float newMult);
    /**
     * @brief Sets a delta time multiplier
     * @param newMult
    */
    const float & GetTimeMultiplier() const;

    /**
     * @brief Initializes window and returns it
     * @return window instance, nullptr if any errors occured
    */
    static Window * Init(const char * windowName, const char * iconPath = nullptr);
    /**
     * @brief Gets window instance
     * @return window
    */
    static Window * Get();

public:
    /// @brief GLFW Window
    GLFWwindow * window;
    /// @brief FPS cap (0 means no cap)
    unsigned int fpsCap;
private:
    GLboolean __initialized;
    float __deltaTime, __deltaTimeMultiplier;
};
