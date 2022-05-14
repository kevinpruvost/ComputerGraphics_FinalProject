/*****************************************************************//**
 * \file   Constants.hpp
 * \brief  Contains all constants/constexpr variables associated
 *         with settings
 * 
 * \author Kevin Pruvost (pruvostkevin0@gmail.com)
 * \date   April, 04 2022
 *********************************************************************/
#pragma once

namespace Constants
{
namespace Window
{
constexpr const char * windowName = "Final Project: Pruvost Kevin 2021400603";
}; // !Constants::Window

namespace Paths
{
// Window
constexpr const char * windowIcon            = "resources/Icons/tsinghua_icon.png";
// Shaders
constexpr const char * pointShaderVertex     = "resources/Shaders/point.vert.glsl";
constexpr const char * pointShaderFrag       = "resources/Shaders/point.frag.glsl";
constexpr const char * faceShaderVertex      = "resources/Shaders/face.vert.glsl";
constexpr const char * faceShaderFrag        = "resources/Shaders/face.frag.glsl";
constexpr const char * face2DShaderVertex    = "resources/Shaders/face2D.vert.glsl";
constexpr const char * face2DShaderFrag      = "resources/Shaders/face2D.frag.glsl";
constexpr const char * wireframeShaderVertex = "resources/Shaders/wireframe.vert.glsl";
constexpr const char * wireframeShaderFrag   = "resources/Shaders/wireframe.frag.glsl";
constexpr const char * lightShaderVertex     = "resources/Shaders/light.vert.glsl";
constexpr const char * lightShaderFrag       = "resources/Shaders/light.frag.glsl";

constexpr const char * text2DShaderVertex    = "resources/Shaders/text2D.vert.glsl";
constexpr const char * text2DShaderFrag      = "resources/Shaders/text2D.frag.glsl";
constexpr const char * text3DShaderVertex    = "resources/Shaders/text3D.vert.glsl";
constexpr const char * text3DShaderFrag      = "resources/Shaders/text3D.frag.glsl";

constexpr const char * particleShaderVertex = "resources/Shaders/particle.vert.glsl";
constexpr const char * particleShaderFrag   = "resources/Shaders/particle.frag.glsl";
constexpr const char * snowShaderVertex     = "resources/Shaders/snow.vert.glsl";
constexpr const char * snowShaderFrag       = "resources/Shaders/snow.frag.glsl";

constexpr const char * bezierShaderFrag   = "resources/Shaders/bezier.frag.glsl";
constexpr const char * bezierShaderVertex = "resources/Shaders/bezier.vert.glsl";
constexpr const char * bezierShaderTcs    = "resources/Shaders/bezier.tcs.glsl";
constexpr const char * bezierShaderTes    = "resources/Shaders/bezier.tes.glsl";

constexpr const char * bezierWireframeShader = "bezier_wireframe";
constexpr const char * bezierWireframeShaderFrag   = "resources/Shaders/wireframe.frag.glsl";
constexpr const char * bezierWireframeShaderVertex = "resources/Shaders/wireframe.vert.glsl";
constexpr const char * bezierWireframeShaderTcs    = "resources/Shaders/bezier.tcs.glsl";
constexpr const char * bezierWireframeShaderTes    = "resources/Shaders/bezier.tes.glsl";

constexpr const char * axisDisplayerShaderVertex = "resources/Shaders/axisdisplayer.vert.glsl";
constexpr const char * axisDisplayerShaderFrag   = "resources/Shaders/axisdisplayer.frag.glsl";
// Planets
constexpr const char * star = "resources/Textures/Star.bmp";
// Snow
constexpr const char * snowflake       = "resources/Textures/snow2.png";
constexpr const char * snowyBackground = "resources/Textures/snowy_background.png";
// Bezier
constexpr const char * ground1 = "resources/Textures/ground1.png";
// Fonts
constexpr const char * arialFont = "resources/Fonts/arial.ttf";
constexpr const char * starFont  = "resources/Fonts/star.ttf";
constexpr const char * notesFont = "resources/Fonts/notes.ttf";
namespace Models
{
namespace Rat
{
constexpr const char * objFile  = "resources/Models/real_rat.obj";
constexpr const char * material = "resources/Models/real_rat.mtl";
constexpr const char * texture  = "resources/Models/rat.png";
}; // !Constants::Paths::Models::Rat
namespace Torus
{
constexpr const char * objFile = "resources/Models/eight.uniform.obj";
}; // !Constants::Paths::Models::Torus
namespace Bunny
{
constexpr const char * objFile = "resources/Models/Bunny.obj";
}; // !Constants::Paths::Models::Bunny
namespace Cube
{
constexpr const char * objFile = "resources/Models/cube.obj";
}; // !Constants::Paths::Models::Cube
namespace Icosahedron
{
constexpr const char * objFile = "resources/Models/Icosahedron.obj";
}; // !Constants::Paths::Models::Icosahedron
}; // !Constants::Paths::Models
}; // !Constants::Paths

namespace UBO // Uniform Buffer Objects
{
namespace Names
{
constexpr const char * cameraProps = "CameraProps";
constexpr const char * lights      = "Lights";
constexpr const char * projection  = "Projection";
}; // !Constants::UBO::Names
namespace Ids
{
constexpr const GLuint cameraProps = 0;
constexpr const GLuint lights = 1;
constexpr const GLuint projection = 2;
};
}; // !Constants::UBO
}; // !Constants