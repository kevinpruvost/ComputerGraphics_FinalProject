/*****************************************************************//**
 * \file   HDRTexture.hpp
 * \brief  HDRTexture Loading Class
 * 
 * \author Kevin Pruvost (pruvostkevin0@gmail.com)
 * \date   May, 18 2022
 *********************************************************************/
#pragma once

// GLAD includes
#include <glad\glad.h>

// GLM includes
#include <glm\glm.hpp>

// C++ includes
#include <string>

/**
 * @brief Contains and manages every information about textures
*/
class HDRTexture
{
public:
    HDRTexture();
    ~HDRTexture();

    /**
     * @brief Generates texture from texture file path
     * @param filePath 
     * @return true if no errors else false
    */
    bool GenerateTexture(const std::string & filePath, int forceChannels = 0);
    
    /**
     * @brief Returns texture width
     * @return width
    */
    int GetWidth() const;
    /**
     * @brief Returns texture height
     * @return height
    */
    int GetHeight() const;
    /**
     * @brief Returns (width, height) texture dimensions
     * @return ivec2 of (width, height)
    */
    glm::ivec2 GetDimensions() const;
    /**
     * @brief Returns texture id (from OpenGL)
     * @return texture id
    */
    GLuint GetTexture() const;

private:
    int __width, __height;
    GLuint __textureId;
};

/**
 * @brief Generates texture from filepath and ref to a Texture & object
 * @param filePath
 * @param texture ref
 * @return true if no errors else flase
 */
bool GenerateHDRTexture(const std::string & filePath, HDRTexture & texture);