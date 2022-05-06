/*****************************************************************//**
 * \file   Font.cpp
 * \brief  Font source code
 * 
 * \author Kevin Pruvost (pruvostkevin0@gmail.com)
 * \date   April, 06 2022
 *********************************************************************/
#include "Font.hpp"

// C++ includes
#include <memory>

static std::vector<std::unique_ptr<Font_Base>> fontDB;

Font::Font(const GLuint fontId)
    : __fontId{ fontId }
{
}

const std::unordered_map<GLchar, std::unique_ptr<Character>> & Font::GetCharacters() const
{
    return fontDB[__fontId]->GetCharacters();
}

GLuint Font::GetFontDatabaseID() const
{
    return __fontId;
}

FT_UInt Font::GetFontSize() const
{
    return fontDB[__fontId]->GetFontSize();
}

Font GenerateFont(const char * fontPath)
{
    fontDB.emplace_back(new Font_Base(fontPath));
    return Font(fontDB.size() - 1);
}
