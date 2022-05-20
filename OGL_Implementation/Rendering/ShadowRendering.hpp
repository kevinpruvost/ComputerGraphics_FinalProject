/*****************************************************************//**
 * \file   ShadowRendering.hpp
 * \brief  Shadow Mapping class
 * 
 * \author Kevin Pruvost (pruvostkevin0@gmail.com)
 * \date   May, 20 2022
 *********************************************************************/
#pragma once

// GLAD includes
#include <GLAD\glad.h>

// GLM includes
#include <GLM\glm.hpp>

class ShadowRendering
{
public:
    ShadowRendering();
    ~ShadowRendering();

    static void Init();
    static void CalculateShadowRendering();

private:
    unsigned int depthMapFBO;
    unsigned int depthMap;
};
