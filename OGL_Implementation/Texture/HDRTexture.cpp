/*****************************************************************//**
 * \file   HDRTexture.cpp
 * \brief  Source code of the Texture class
 * 
 * \author Kevin Pruvost (pruvostkevin0@gmail.com)
 * \date   April, 01 2022
 *********************************************************************/
#include "HDRTexture.hpp"

// STBI includes
#include <stb_image.h>

HDRTexture::HDRTexture()
    : __width{ 0 }
    , __height{ 0 }
{
}

HDRTexture::~HDRTexture()
{
    glDeleteTextures(1, &__textureId);
}

bool HDRTexture::GenerateTexture(const std::string & filePath, int forceChannels)
{
    int nrComponents;
    stbi_set_flip_vertically_on_load(true);
    float * image = stbi_loadf(filePath.c_str(), &__width, &__height, &nrComponents, forceChannels);
    stbi_set_flip_vertically_on_load(false);
    if (image)
    {
        glGenTextures(1, &__textureId);
        glBindTexture(GL_TEXTURE_2D, __textureId);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, __width, __height, 0, GL_RGB, GL_FLOAT, image); // note how we specify the texture's data value to be float

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(image);
        return true;
    }
    return false;
}

int HDRTexture::GetWidth() const
{
    return __width;
}

int HDRTexture::GetHeight() const
{
    return __height;
}

glm::ivec2 HDRTexture::GetDimensions() const
{
    return glm::ivec2(__width, __height);
}

GLuint HDRTexture::GetTexture() const
{
    return __textureId;
}

bool GenerateHDRTexture(const std::string & filePath, HDRTexture & texture)
{
    return texture.GenerateTexture(filePath);
}
