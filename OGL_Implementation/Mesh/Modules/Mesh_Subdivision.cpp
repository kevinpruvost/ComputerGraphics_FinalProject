/*****************************************************************//**
 * \file   Mesh_Subdivision.cpp
 * \brief  Mesh Subdivision source code
 *
 * \author Kevin Pruvost (pruvostkevin0@gmail.com)
 * \date   May, 06 2022
 *********************************************************************/
#include "Mesh_Subdivision.hpp"

// Project includes
#include "OGL_Implementation\DebugInfo\Log.hpp"
#include "OGL_Implementation\DebugInfo\Timer.hpp"

// C++ includes
#include <unordered_map>
#include <chrono>
using namespace std::chrono_literals;

Mesh_Custom * Mesh_Subdivision::Subdivide(Mesh_Base & mesh)
{
    Timer timer;
    timer.Start();

    const std::vector<Face> originFaces(*mesh.GetFaces());
    const std::vector<VertexPos> originVertices(*mesh.GetVerticesPos());
    std::vector<Face> newFaces;
    newFaces.reserve(originFaces.size() * 4);
    std::vector<VertexPos> newVertices(originVertices);
    newVertices.reserve(originVertices.size() * 3);

    const std::vector<std::unique_ptr<HalfEdge>> halfEdges = GenerateHalfEdgesFromVertices(originFaces);
    std::vector<std::unique_ptr<HalfEdge>> newHalfEdges;
    std::unordered_map<HalfEdge *, int> reUseVertex;
    newHalfEdges.reserve(halfEdges.size() * 4);
    // Creating new Vertices and new Triangles
    for (int i = 0; i < halfEdges.size(); i += 3)
    {
        int vertex1 = -1, vertex2 = -1, vertex3 = -1;
        const auto searchForVertex = [&](int & vertex, HalfEdge * halfEdge) {
            if (halfEdge->pass)
            {
                halfEdge->pass = false;
                vertex = reUseVertex.at(halfEdge);
            }
            if (vertex == -1)
            {
                const VertexPos & v1 = newVertices[halfEdge->origin];
                const VertexPos & v2 = newVertices[halfEdge->next->origin];
                VertexPos newPos;
                if (halfEdge->twin)
                {
                    const VertexPos & v3 = newVertices[halfEdge->previous->origin];
                    const VertexPos & v4 = newVertices[halfEdge->twin->previous->origin];
                    newPos = (v1 + v2) * 0.375f + (v3 + v4) * 0.125f;
                }
                else
                {
                    newPos = (v1 + v2) * 0.5f;
                }
                newVertices.emplace_back(newPos);
                vertex = newVertices.size() - 1;
                if (halfEdge->twin)
                {
                    halfEdge->twin->pass = true;
                    reUseVertex.emplace(halfEdge->twin, vertex);
                }
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

        const auto newEdges1 = createTriangle(halfEdges[i]->origin, vertex1, vertex3);
        const auto newEdges2 = createTriangle(halfEdges[i + 1]->origin, vertex2, vertex1);
        const auto newEdges3 = createTriangle(halfEdges[i + 2]->origin, vertex3, vertex2);
        const auto newEdges4 = createTriangle(vertex1, vertex2, vertex3);

        // Linking inner twins
        newEdges1[1]->twin = newEdges4[0]; newEdges4[0]->twin = newEdges1[1];
        newEdges2[1]->twin = newEdges4[1]; newEdges4[1]->twin = newEdges1[1];
        newEdges3[1]->twin = newEdges4[2]; newEdges4[2]->twin = newEdges3[1];

        const std::array<HalfEdge *, 6> remainingTwins = {newEdges1[0], newEdges1[2], newEdges2[0], newEdges2[2], newEdges3[0], newEdges3[2]};
        for (int j = 0; j < newHalfEdges.size(); ++j)
        {
            if (newHalfEdges[j]->twin)
            {
                continue;
            }
            for (int x = 0; x < 6; ++x)
            {
                if (newHalfEdges[j]->origin == remainingTwins[x]->next->origin && newHalfEdges[j]->next->origin == remainingTwins[x]->origin)
                {
                    newHalfEdges[j]->twin = remainingTwins[x];
                    remainingTwins[x]->twin = newHalfEdges[j].get();
                    break;
                }
            }
        }
    }
    // Calculating new positions
    {
        // Old vertices
        // Mapping all u concerned by old vertex
        std::unordered_map<int, std::vector<int>> verticesPerOldVertex;
        for (int i = 0; i < originFaces.size(); ++i)
        {
            const auto insertVertex = [&](int oldVertex, int vertex) {
                std::unordered_map<int, std::vector<int>>::iterator ite;
                if ((ite = verticesPerOldVertex.find(oldVertex)) == verticesPerOldVertex.end())
                    verticesPerOldVertex[oldVertex] = std::vector<int>({ vertex });
                else
                    ite->second.push_back(vertex);
            };

            for (int j = 0; j < 3; ++j)
            {
                for (int x = 0; x < 3; ++x)
                {
                    if (x == j) continue;
                    insertVertex(originFaces[i].v[j], originFaces[i].v[x]);
                }
            }
        }
        for (auto ite = verticesPerOldVertex.begin(); ite != verticesPerOldVertex.end(); ++ite)
        {
            int n = ite->second.size();
            if (n == 2)
            {
                newVertices[ite->first] = 0.75f * originVertices[ite->first] + 0.125f * (originVertices[ite->second[0]] + originVertices[ite->second[1]]);
                continue;
            }
            // n = 3 -> 3/16 else 3/8n
            float beta = n == 3 ? 0.1875f : (3.0f / (8.0f * n));
            VertexPos sum = glm::vec3(0.0f);
            for (int i = 0; i < n; ++i)
                sum += originVertices[ite->second[i]];
            auto part1 = (1.0f - n * beta) * originVertices[ite->first];
            auto part2 = beta * sum;
            newVertices[ite->first] = (1.0f - n * beta) * originVertices[ite->first] + beta * sum;
        }
    }
    Mesh_Custom * newMesh = new Mesh_Custom(newVertices, newFaces);
    newMesh->GenerateNormals(true);
    Log::Print(Log::LogMainFileName, "Final Time Subdivision: %.2fms\n", timer.GetMsTime());
    return newMesh;
}

void Mesh_Subdivision::SubdivideParallel(Mesh_Base & mesh)
{
    const auto threadFunc = [](Mesh_Base * mesh, bool * finished, bool * loopFinished, bool * working, bool * abort, std::condition_variable * cv, std::mutex * mutex) {
        Timer timer;
        const std::vector<Face> originFaces(*mesh->GetFaces());
        const std::vector<VertexPos> originVertices(*mesh->GetVerticesPos());
        std::vector<Face> newFaces;
        newFaces.reserve(originFaces.size() * 4);
        std::vector<VertexPos> newVertices(originVertices);
        newVertices.reserve(originVertices.size() * 3);

        const std::vector<std::unique_ptr<HalfEdge>> halfEdges = GenerateHalfEdgesFromVertices(originFaces);
        std::vector<std::unique_ptr<HalfEdge>> newHalfEdges;
        std::unordered_map<HalfEdge *, int> reUseVertex;
        newHalfEdges.reserve(halfEdges.size() * 4);
        // Creating new Vertices and new Triangles
        for (int i = 0; i < halfEdges.size(); i += 3)
        {
            int vertex1 = -1, vertex2 = -1, vertex3 = -1;
            const auto searchForVertex = [&](int & vertex, HalfEdge * halfEdge) {
                if (halfEdge->pass)
                {
                    halfEdge->pass = false;
                    vertex = reUseVertex.at(halfEdge);
                }
                if (vertex == -1)
                {
                    const VertexPos & v1 = newVertices[halfEdge->origin];
                    const VertexPos & v2 = newVertices[halfEdge->next->origin];
                    VertexPos newPos;
                    if (halfEdge->twin)
                    {
                        const VertexPos & v3 = newVertices[halfEdge->previous->origin];
                        const VertexPos & v4 = newVertices[halfEdge->twin->previous->origin];
                        newPos = (v1 + v2) * 0.375f + (v3 + v4) * 0.125f;
                    }
                    else
                    {
                        newPos = (v1 + v2) * 0.5f;
                    }
                    newVertices.emplace_back(newPos);
                    vertex = newVertices.size() - 1;
                    if (halfEdge->twin)
                    {
                        halfEdge->twin->pass = true;
                        reUseVertex.emplace(halfEdge->twin, vertex);
                    }
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

            const auto newEdges1 = createTriangle(halfEdges[i]->origin, vertex1, vertex3);
            const auto newEdges2 = createTriangle(halfEdges[i + 1]->origin, vertex2, vertex1);
            const auto newEdges3 = createTriangle(halfEdges[i + 2]->origin, vertex3, vertex2);
            const auto newEdges4 = createTriangle(vertex1, vertex2, vertex3);

            // Linking inner twins
            newEdges1[1]->twin = newEdges4[0]; newEdges4[0]->twin = newEdges1[1];
            newEdges2[1]->twin = newEdges4[1]; newEdges4[1]->twin = newEdges1[1];
            newEdges3[1]->twin = newEdges4[2]; newEdges4[2]->twin = newEdges3[1];

            const std::array<HalfEdge *, 6> remainingTwins = { newEdges1[0], newEdges1[2], newEdges2[0], newEdges2[2], newEdges3[0], newEdges3[2] };
            for (int j = 0; j < newHalfEdges.size(); ++j)
            {
                if (newHalfEdges[j]->twin)
                {
                    continue;
                }
                for (int x = 0; x < 6; ++x)
                {
                    if (newHalfEdges[j]->origin == remainingTwins[x]->next->origin && newHalfEdges[j]->next->origin == remainingTwins[x]->origin)
                    {
                        newHalfEdges[j]->twin = remainingTwins[x];
                        remainingTwins[x]->twin = newHalfEdges[j].get();
                        break;
                    }
                }
            }
            if (*abort) return;
            if (*working)
            {
                std::unique_lock<std::mutex> lk(*mutex);
                cv->wait_for(lk, 3s, [&] { return !*working; });
            }
            *working = true;
            mutex->lock();
            mesh->SetGeometry(newFaces, newVertices);
            mutex->unlock();
            *loopFinished = true;
            *working = false;
            cv->notify_all();
        }
        // Calculating new positions
        {
            // Old vertices
            // Mapping all u concerned by old vertex
            std::unordered_map<int, std::vector<int>> verticesPerOldVertex;
            for (int i = 0; i < originFaces.size(); ++i)
            {
                const auto insertVertex = [&](int oldVertex, int vertex) {
                    std::unordered_map<int, std::vector<int>>::iterator ite;
                    if ((ite = verticesPerOldVertex.find(oldVertex)) == verticesPerOldVertex.end())
                        verticesPerOldVertex[oldVertex] = std::vector<int>({ vertex });
                    else
                        ite->second.push_back(vertex);
                };

                for (int j = 0; j < 3; ++j)
                {
                    for (int x = 0; x < 3; ++x)
                    {
                        if (x == j) continue;
                        insertVertex(originFaces[i].v[j], originFaces[i].v[x]);
                    }
                }
            }
            int nLoop = 0;
            for (auto ite = verticesPerOldVertex.begin(); ite != verticesPerOldVertex.end(); ++ite)
            {
                int n = ite->second.size();
                if (n == 2)
                {
                    newVertices[ite->first] = 0.75f * originVertices[ite->first] + 0.125f * (originVertices[ite->second[0]] + originVertices[ite->second[1]]);
                    continue;
                }
                // n = 3 -> 3/16 else 3/8n
                float beta = n == 3 ? 0.1875f : (3.0f / (8.0f * n));
                VertexPos sum = glm::vec3(0.0f);
                for (int i = 0; i < n; ++i)
                    sum += originVertices[ite->second[i]];
                auto part1 = (1.0f - n * beta) * originVertices[ite->first];
                auto part2 = beta * sum;
                newVertices[ite->first] = (1.0f - n * beta) * originVertices[ite->first] + beta * sum;
                if (++nLoop % ((verticesPerOldVertex.size() / 10) + 1) == 0)
                {
                    if (*abort) return;
                    if (*working)
                    {
                        std::unique_lock<std::mutex> lk(*mutex);
                        cv->wait_for(lk, 3s, [&] { return !*working; });
                    }
                    *working = true;
                    mutex->lock();
                    mesh->SetGeometry(newFaces, newVertices);
                    mutex->unlock();
                    *loopFinished = true;
                    *working = false;
                    cv->notify_all();
                }
            }
        }
        if (*abort) return;
        if (*working)
        {
            std::unique_lock<std::mutex> lk(*mutex);
            cv->wait_for(lk, 3s, [&] { return !*working; });
        }
        *working = true;
        mutex->lock();
        mesh->SetGeometry(newFaces, newVertices);
        mutex->unlock();
        *finished = true;
        *working = false;
        cv->notify_all();
        Log::Print(Log::LogMainFileName, "Final Time Subdivision: %.2fms\n", timer.GetMsTime());
    };
    Mesh_ThreadPool::SetNewThread(&mesh, threadFunc);
}