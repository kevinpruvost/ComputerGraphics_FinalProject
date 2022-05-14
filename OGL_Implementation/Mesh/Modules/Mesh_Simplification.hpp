/*****************************************************************//**
 * \file   Mesh_Simplification.hpp
 * \brief  Mesh Simplifier
 * 
 * \author Kevin Pruvost (pruvostkevin0@gmail.com)
 * \date   May, 06 2022
 *********************************************************************/
#pragma once

// Project includes
#include "../Mesh_Custom.hpp"
#include "Mesh_ThreadPool.hpp"

// C++ includes
#include <map>
#include <mutex>

/**
 * @brief Part of Mesh class, Simplifies the mesh with Quadric Error Metrics
*/
class Mesh_Simplification
{
private:
    static void GeneratePlanes(const std::vector<VertexPos> & vPs, const std::vector<Face> & originFaces, std::vector<glm::mat4> & planes);
    static void GenerateQMatrices(const std::vector<VertexPos> & vPs, const std::vector<Face> & originFaces, const std::vector<glm::mat4> & planes, std::vector<glm::mat4> & qMatrices);
    static std::map<float, HalfEdge *> GenerateErrorMetrics(const std::vector<VertexPos> & vPs, const std::vector<Face> & originFaces, std::vector<std::unique_ptr<HalfEdge>> & halfEdges, const std::vector<glm::mat4> & qMatrices);

private:

public:
    static void SimplifyParallel(Mesh_Base & mesh);
    static Mesh_Custom * Simplify(Mesh_Base & mesh);
};
