/*****************************************************************//**
 * \file   Mesh_Subdivision.cpp
 * \brief  Mesh Subdivision source code
 *
 * \author Kevin Pruvost (pruvostkevin0@gmail.com)
 * \date   May, 06 2022
 *********************************************************************/
#include "Mesh_Subdivision.hpp"

Mesh_Subdivision::Mesh_Subdivision()
{
}

Mesh_Subdivision::~Mesh_Subdivision()
{
}

Mesh_Custom * Mesh_Subdivision::__Subdivide(Mesh_Base & mesh)
{
    const std::vector<Face> originFaces(*mesh.GetFaces());
    const std::vector<VertexPos> originVertices(*mesh.GetVerticesPos());
    std::vector<Face> newFaces;
    newFaces.reserve(originFaces.size() * 4);
    std::vector<VertexPos> newVertices(originVertices);
    newVertices.reserve(originVertices.size() * 2);

    const std::vector<std::unique_ptr<HalfEdge>> halfEdges = GenerateHalfEdgesFromVertices(originFaces);
    std::vector<std::unique_ptr<HalfEdge>> newHalfEdges;
    newHalfEdges.reserve(halfEdges.size() * 4);
    // Creating new Vertices and new Triangles
    int twinsAccelerator = 0;
    for (int i = 0; i < halfEdges.size(); i += 3)
    {
        int vertex1 = -1, vertex2 = -1, vertex3 = -1;
        const auto searchForVertex = [&](int & vertex, HalfEdge * halfEdge) {
            if (halfEdge->pass)
            {
                halfEdge->pass = false;
                for (int j = twinsAccelerator; j < newHalfEdges.size(); ++j)
                {
                    if (newHalfEdges[j]->twin) continue;
                    if (halfEdge->next->origin == newHalfEdges[j]->origin && halfEdge->origin == newHalfEdges[j]->next->twin->next->twin->next->next->origin)
                    {
                        vertex = newHalfEdges[j]->next->origin;
                        break;
                    }
                }
            }
            if (vertex == -1)
            {
                newVertices.emplace_back((newVertices[halfEdge->origin] + newVertices[halfEdge->next->origin]) / 2.0f);
                vertex = newVertices.size() - 1;
                if (halfEdge->twin) halfEdge->twin->pass = true;
            }
        };
        // Search for already existing vertices
        // Vertices
        searchForVertex(vertex1, halfEdges[i].get());
        searchForVertex(vertex2, halfEdges[i + 1].get());
        searchForVertex(vertex3, halfEdges[i + 2].get());

        // Triangles
        const auto createTriangle = [&](int v1, int v2, int v3) {
            newFaces.emplace_back(std::vector<int>{ v1, v2, v3 });
            int faceId = newFaces.size() - 1;
            HalfEdge * he1 = newHalfEdges.emplace_back(new HalfEdge(nullptr, nullptr, nullptr, faceId, v1)).get();
            HalfEdge * he2 = newHalfEdges.emplace_back(new HalfEdge(nullptr, nullptr, nullptr, faceId, v2)).get();
            HalfEdge * he3 = newHalfEdges.emplace_back(new HalfEdge(nullptr, nullptr, nullptr, faceId, v3)).get();
            he1->next = he3->previous = he2;
            he2->next = he1->previous = he3;
            he3->next = he2->previous = he1;
            return std::array<HalfEdge *, 3>{he1, he2, he3};
        };

        const auto newEdges1 = createTriangle(vertex3, halfEdges[i]->origin, vertex1);
        const auto newEdges2 = createTriangle(vertex1, halfEdges[i + 1]->origin, vertex2);
        const auto newEdges3 = createTriangle(vertex2, halfEdges[i + 2]->origin, vertex3);
        const auto newEdges4 = createTriangle(vertex1, vertex2, vertex3);

        // Linking inner twins
        newEdges1[2]->twin = newEdges4[0]; newEdges4[0]->twin = newEdges1[2];
        newEdges2[2]->twin = newEdges4[1]; newEdges4[1]->twin = newEdges1[2];
        newEdges3[2]->twin = newEdges4[2]; newEdges4[2]->twin = newEdges3[2];

        const std::array<HalfEdge *, 6> remainingTwins = {newEdges1[0], newEdges1[1], newEdges2[0], newEdges2[1], newEdges3[0], newEdges3[1]};
        for (int j = twinsAccelerator; j < newHalfEdges.size(); ++j)
        {
            if (newHalfEdges[j]->twin)
            {
                if (j == twinsAccelerator) ++twinsAccelerator;
                continue;
            }
            for (int x = 0; x < 6; ++x)
            {
                if (newHalfEdges[j]->origin == remainingTwins[x]->next->origin && newHalfEdges[j]->next->origin == remainingTwins[x]->origin)
                {
                    newHalfEdges[j]->twin = remainingTwins[x];
                    remainingTwins[x]->twin = newHalfEdges[j].get();
                    if (j == twinsAccelerator) ++twinsAccelerator;
                    break;
                }
            }
        }
    }
    // Calculating new positions
    // Old vertices

    // New vertices
    Mesh_Custom * newMesh = new Mesh_Custom(newVertices, newFaces);
    newMesh->GenerateNormals(false);
    return newMesh;
}
