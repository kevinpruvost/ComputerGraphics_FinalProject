/*****************************************************************//**
 * \file   Rendering.hpp
 * \brief  Rendering class
 * 
 * \author Kevin Pruvost (pruvostkevin0@gmail.com)
 * \date   April, 04 2022
 *********************************************************************/
#pragma once

// Project includes
#include "OGL_Implementation\Entity\Entity.hpp"
#include "OGL_Implementation\Text\Text.hpp"
#include "OGL_Implementation\Image\Image2D.hpp"
#include "OGL_Implementation\Cubemap\Brdf_Cubemap.hpp"
#include "LightRendering.hpp"
#include "ParticleSystemRendering.hpp"
#include "Constants.hpp"

/**
 * @brief Static class containing rendering utilities
*/
class Rendering
{
public:
    Rendering();
    ~Rendering();
public:
    GLuint GetTextVAO();
    GLuint GetTextVBO();
    GLuint GetCubeVAO();
    GLuint GetCubeVBO();

private:
    GLuint __textVAO, __textVBO, __cubeVAO, __cubeVBO;

public:
    /**
     * @brief To call to initialize Rendering
    */
    static void Init();
    static void Refresh();

    // Entities
    static void DrawFaces(Entity & entity);
    static void DrawWireframe(Entity & entity);
    static void DrawVertices(Entity & entity);

    static void DrawEntity(Entity & entity);

    static void RotateWireframeColor();

    // Image
    static void DrawImage(Image2D & image);

    // Text
    static void DrawText(Text2D & text);
    static void DrawText(Text3D & text);

    static void RotateFonts();

    // Cubemaps
    static void DrawBrdfCubemap(Brdf_Cubemap & cubemap);

    // ParticleSystem
    static void DrawParticleSystem(ParticleSystem_Base * particleSystem);

private:
    static void LoadShadersAndFonts();
    static void RenderCube();

public:
    static Shader & Shaders(const std::string & str);
    static std::unordered_map<std::string, std::unique_ptr<Shader>> shaders;
};
