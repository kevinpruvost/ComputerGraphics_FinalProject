/*****************************************************************//**
 * \file   Font.hpp
 * \brief  Font class
 * 
 * \author Kevin Pruvost (pruvostkevin0@gmail.com)
 * \date   April, 06 2022
 *********************************************************************/
#pragma once

// Project includes
#include "Font_Base.hpp"

/**
 * @brief Contains and manages every information about Fonts.
*/
class Font
{
public:
    /**
     * @brief Constructor from Shader ID.
     * @param meshId
    */
    Font(const GLuint fontId);

public:
    /**
     * @brief Gets characters data (not mutable)
     * @return characters
    */
    const std::unordered_map<GLchar, std::unique_ptr<Character>> & GetCharacters() const;

    /**
     * @brief Returns Font ID
     * @return font id
    */
    GLuint GetFontDatabaseID() const;

    /**
     * @brief Gets Font size
     * @return font size
    */
    FT_UInt GetFontSize() const;

public:
    GLuint __fontId;
};

/*
 * @brief Generates font from ttf file
 * @param Font Path
*/
Font GenerateFont(const char * fontPath);