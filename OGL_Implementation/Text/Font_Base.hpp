/*****************************************************************//**
 * \file   Font_Base.hpp
 * \brief  Font_Base class
 * 
 * \author Kevin Pruvost (pruvostkevin0@gmail.com)
 * \date   April, 06 2022
 *********************************************************************/
#pragma once

// GLM includes
#include <glm\glm.hpp>

// GLAD includes
#include <glad\glad.h>

// FreeType includes
#include <ft2build.h>
#include <freetype/freetype.h>

// C++ includes
#include <unordered_map>
#include <memory>

/**
 * @brief Contains every information about Rendering Characters.
*/
class Character
{
public:
    Character(const wchar_t character, FT_Face & face, bool & error);
    ~Character();

    GLuint GetTextureID() const;
    const glm::ivec2 & GetSize() const;
    const glm::ivec2 & GetBearing() const;
    GLuint GetAdvance() const;

private:
    GLuint __texture;
    glm::ivec2 __size;    // Size of character
    glm::ivec2 __bearing; // Offset from baseline to left/top of character
    GLuint     __advance; // Horizontal offset to next character
    bool __error;
};

/**
 * @brief Contains the real data of the font to be
 * stored in a static database.
*/
class Font_Base
{
public:
    Font_Base(const char * fontPath, const FT_UInt fontSize = 260);
    ~Font_Base();

    /**
     * @brief Inits FreeType
     * @return true if initialized else false (if any errors happened)
    */
    static bool InitFreeType();

    /**
     * @brief Gets characters data (not mutable)
     * @return characters
    */
    const std::unordered_map<GLchar, std::unique_ptr<Character>> & GetCharacters() const;

    /**
     * @brief Gets Font size
     * @return font size
    */
    FT_UInt GetFontSize() const;

private:
    FT_Face __face;
    FT_UInt __fontSize;
    std::unordered_map<GLchar, std::unique_ptr<Character>> __characters;
};
