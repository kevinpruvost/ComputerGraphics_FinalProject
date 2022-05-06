/*****************************************************************//**
 * \file   RenderingSettings.hpp
 * \brief  Rendering enum & settings code related
 * 
 * \author Kevin Pruvost (pruvostkevin0@gmail.com)
 * \date   April, 16 2022
 *********************************************************************/
#pragma once

// GLM includes
#include <glm\glm.hpp>

// C++ includes
#include <array>

/**
 * @brief Enum of every way to display a mesh (points, wireframes, faces & mixes between them)
*/
enum RenderingMode
{
    VerticesMode             = 0b001,
    WireframeMode            = 0b010,
    VerticesAndWireframeMode = 0b011,
    FacesMode                = 0b100,
    VerticesAndFacesMode     = 0b101,
    FacesAndWireframeMode    = 0b110,
    AllRenderingModes        = 0b111
};

// Display Mode
extern RenderingMode DisplayMode; // 0 = Point, 1 = Face, 2 = Wireframe, 3 = Face + Wireframe
extern std::array<glm::vec3, 4> WireframeColors;