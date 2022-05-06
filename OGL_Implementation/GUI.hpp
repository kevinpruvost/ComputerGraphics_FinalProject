/*****************************************************************//**
 * \file   GUI.hpp
 * \brief  GUI class using DearImGui
 * 
 * \author Kevin Pruvost (pruvostkevin0@gmail.com)
 * \date   March, 27 2022
 *********************************************************************/
#pragma once

// Project includes
#include "OGL_Implementation\Entity\Entity.hpp"

// GLM includes
#include <glm\glm.hpp>

// ImGui includes
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

// C++ includes
#include <functional>
#include <vector>

/**
 * @brief Manages ImGUI UI in an easier and more practical way.
*/
class GUI
{
public:
    /**
     * @brief Constructor, only needs OpenGL window.
     * @param window 
    */
    GUI(GLFWwindow * window);
    ~GUI();

    /**
     * @brief To call in the loop to draw the whole GUI.
     * @return false if there's an error otherwise true
    */
    bool DrawGUI();
    /**
     * @brief Adds a callback to be called from DrawGUI.
     * @param lambda 
    */
    void AddCallback(const std::function<bool()> & lambda);

    void EditEntity(Entity & entity);

public:

private:
    ImGuiIO * io;
    std::vector<std::function<bool()>> __callbacks;
};
