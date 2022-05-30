/*****************************************************************//**
 * \file   Light.cpp
 * \brief  Light source code
 * 
 * \author Kevin Pruvost (pruvostkevin0@gmail.com)
 * \date   April, 12 2022
 *********************************************************************/
#include "LightRendering.hpp"

// Project includes
#include "Constants.hpp"
#include "OGL_Implementation\Window.hpp"

// C++ includes
#include <format>

static std::unique_ptr<LightRendering> s_lightRendering;
static const size_t shadowWidth = 2048, static const shadowHeight = 2048;

#include <stb_image_write.h>

LightRendering::LightRendering()
    : __uboLights{ 0 }
    , __shadowMappingShader{ GenerateShader(Constants::Paths::shadowMappingVertex, Constants::Paths::shadowMappingFrag) }
    , screenshotDepthMap{ false }
{
    // Allocating UBO ViewProj
    glGenBuffers(1, &__uboLights);

    constexpr const size_t LightsSize = sizeof(PointLight_Shader) * PointLight::maxPointLightsCount
//      + sizeof(DirectionLight_Shader)
//      + sizeof(SpotLight_Shader)
      + sizeof(int);
    glBindBuffer(GL_UNIFORM_BUFFER, __uboLights);
    glBufferData(GL_UNIFORM_BUFFER, LightsSize, NULL, GL_DYNAMIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    // Binds buffer to a specific binding point so that it'll be used at this exact place
    // by shaders
    glBindBufferRange(GL_UNIFORM_BUFFER, Constants::UBO::Ids::lights, __uboLights, 0, LightsSize);

    ////////////////////////
    // Binding shadow maps
    ////////////////////////
    glGenTextures(PointLight::maxPointLightsCount, shadowMaps.data());
    glGenFramebuffers(PointLight::maxPointLightsCount, __depthMapFbo.data());
    // create depth texture
    for (int i = 0; i < PointLight::maxPointLightsCount; ++i)
    {
        glBindTexture(GL_TEXTURE_2D, shadowMaps[i]);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, shadowWidth, shadowHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
        constexpr const float borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
        glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
        // attach depth texture as FBO's depth buffer
        glBindFramebuffer(GL_FRAMEBUFFER, __depthMapFbo[i]);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, shadowMaps[i], 0);
        glDrawBuffer(GL_NONE);
        glReadBuffer(GL_NONE);
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

LightRendering::~LightRendering()
{
    glDeleteBuffers(1, &__uboLights);
    glDeleteTextures(PointLight::maxPointLightsCount, shadowMaps.data());
    glDeleteFramebuffers(PointLight::maxPointLightsCount, __depthMapFbo.data());
}

GLuint LightRendering::GetUboLights()
{
    return __uboLights;
}

Shader & LightRendering::GetShadowMappingShader()
{
    return __shadowMappingShader;
}

const std::array<GLuint, PointLight::maxPointLightsCount> & LightRendering::GetDepthMapFbo() const
{
    return __depthMapFbo;
}

void LightRendering::Init()
{
    s_lightRendering.reset(new LightRendering());
}

const LightRendering & LightRendering::Get()
{
    if (s_lightRendering) return *s_lightRendering;
    throw std::runtime_error("Trying to get LightRendering although it has not been initialized.\n");
}

void LightRendering::RefreshUbo()
{
    const int pointLightsCount = PointLight::GetPointLightsCount();
    const auto & pointLights = PointLight::GetAllPointLights();
    const auto & entities = Entity::GetAllEntities();

    glBindBuffer(GL_UNIFORM_BUFFER, s_lightRendering->GetUboLights());

    Shader & shader = s_lightRendering->GetShadowMappingShader();
    const std::array<GLuint, PointLight::maxPointLightsCount> & framebuffers = s_lightRendering->GetDepthMapFbo();
    shader.Use();

    glViewport(0, 0, shadowWidth, shadowHeight);
    for (size_t i = 0; i < pointLightsCount; ++i)
    {
        auto shaderInfo = pointLights[i]->GetShaderInfo();
        glBufferSubData(GL_UNIFORM_BUFFER, i * sizeof(PointLight_Shader), sizeof(PointLight_Shader), &shaderInfo);

        shader.SetUniformMatrix4f("lightSpaceMatrix", shaderInfo.pointLightViewMatrix);

        glBindFramebuffer(GL_FRAMEBUFFER, framebuffers[i]);
        glClear(GL_DEPTH_BUFFER_BIT);
        for (int i = 0; i < entities.size(); ++i)
        {
            if (dynamic_cast<PointLight *>(entities[i])) continue;

            shader.SetUniformMatrix4f("model", entities[i]->GetModelMatrix());
            glBindVertexArray(entities[i]->GetMesh().facesVAO());

            glDrawArrays(GL_TRIANGLES, 0, entities[i]->GetMesh().facesNVert());
        }
    }

    if (s_lightRendering->screenshotDepthMap)
    {
        GLubyte * pixels = new GLubyte[shadowWidth * shadowHeight * 1];
        glPixelStorei(GL_PACK_ALIGNMENT, 1);
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        glReadPixels(0, 0, shadowWidth, shadowHeight, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, pixels);
        stbi_write_bmp("screenshots/TEST.bmp", shadowWidth, shadowHeight, 1, pixels);
        delete[] pixels;
        s_lightRendering->screenshotDepthMap = false;
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, Window::Get()->windowWidth(), Window::Get()->windowHeight());
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glBufferSubData(GL_UNIFORM_BUFFER, sizeof(PointLight_Shader) * PointLight::maxPointLightsCount,
        sizeof(int), &pointLightsCount);
}

void LightRendering::PrintDepthMap()
{
    s_lightRendering->screenshotDepthMap = true;
}
