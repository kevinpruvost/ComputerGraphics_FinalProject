/*****************************************************************//**
 * \file   ShadowRendering.cpp
 * \brief  Shadow Mapping source code
 * 
 * \author Kevin Pruvost (pruvostkevin0@gmail.com)
 * \date   May, 20 2022
 *********************************************************************/
#include "ShadowRendering.hpp"

// C++ includes
#include <memory>

static std::unique_ptr<ShadowRendering> s_ShadowRendering(nullptr);

ShadowRendering::ShadowRendering()
{
}

ShadowRendering::~ShadowRendering()
{
}

void ShadowRendering::Init()
{
    s_ShadowRendering.reset(new ShadowRendering);
}
