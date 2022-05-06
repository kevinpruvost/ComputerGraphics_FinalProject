/*****************************************************************//**
 * \file   Texture.cpp
 * \brief  Source code of the Texture class
 * 
 * \author Kevin Pruvost (pruvostkevin0@gmail.com)
 * \date   April, 01 2022
 *********************************************************************/
#include "Texture.hpp"

Texture::Texture()
    : __width{ 0 }
    , __height{ 0 }
{
}

Texture::~Texture()
{
}

bool Texture::GenerateTexture(const std::string & filePath)
{
    auto * image = SOIL_load_image(filePath.c_str(), &__width, &__height, nullptr, SOIL_LOAD_RGB);
    if (image)
    {
        // Load and create a texture
        glGenTextures(1, &__textureId);
        glBindTexture(GL_TEXTURE_2D, __textureId); // All upcoming GL_TEXTURE_2D operations now have effect on this texture object
        // Set the texture wrapping parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);    // Set texture wrapping to GL_REPEAT (usually basic wrapping method)
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        // Set texture filtering parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        // Load image, create texture and generate mipmaps
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, __width, __height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
        glGenerateMipmap(GL_TEXTURE_2D);
        // Unbind texture when done, so we won't accidentily mess up our texture.
        glBindTexture(GL_TEXTURE_2D, 0);
        SOIL_free_image_data(image);
        return true;
    }
    return false;
}

int Texture::GetWidth() const
{
    return __width;
}

int Texture::GetHeight() const
{
    return __height;
}

glm::ivec2 Texture::GetDimensions() const
{
    return glm::ivec2(__width, __height);
}

GLuint Texture::GetTexture() const
{
    return __textureId;
}

bool GenerateTexture(const std::string & filePath, Texture & texture)
{
    return texture.GenerateTexture(filePath);
}
