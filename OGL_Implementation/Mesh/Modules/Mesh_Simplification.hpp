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

// C++ includes
#include <map>
#include <mutex>

class Mesh_Simplification
{
public:
    Mesh_Simplification();
    ~Mesh_Simplification();

private:
    void GeneratePlanes(const std::vector<VertexPos> & vPs, const std::vector<Face> & originFaces);
    void GenerateQMatrices(const std::vector<VertexPos> & vPs, const std::vector<Face> & originFaces);
    std::map<float, HalfEdge *> GenerateErrorMetrics(const std::vector<VertexPos> & vPs, const std::vector<Face> & originFaces, std::vector<std::unique_ptr<HalfEdge>> & halfEdges);

private:
    std::vector<glm::mat4> qMatrices;
    std::vector<glm::mat4> planes;

protected:
    void __SimplifyParallel(Mesh_Base & mesh, bool &loopPassed, std::mutex * mutex);
    Mesh_Custom * __Simplify(Mesh_Base & mesh);
};
