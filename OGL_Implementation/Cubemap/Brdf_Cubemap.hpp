/*****************************************************************//**
 * \file   Brdf_Cubemap.hpp
 * \brief  Brdf_Cubemap class
 * 
 * \author Kevin Pruvost (pruvostkevin0@gmail.com)
 * \date   May, 18 2022
 *********************************************************************/
#pragma once

// Project includes
#include "OGL_Implementation\Shader\Shader.hpp"
#include "OGL_Implementation\Texture\HDRTexture.hpp"
#include "OGL_Implementation\EntityAttribute\EntityAttribute.hpp"

class Brdf_Cubemap
{
public:
    Brdf_Cubemap(const std::string & hdrTexturePath, const Shader & backgroundShader_);
    ~Brdf_Cubemap();

private:
    void RenderCube();
    void RenderQuad();

public:
    GLuint cubemapTexture;
    GLuint irradianceMap;
    GLuint prefilterMap;
    GLuint brdfLUTTexture;
    HDRTexture texture;
    Shader shader;
};

extern Brdf_Cubemap * s_cubemap;
