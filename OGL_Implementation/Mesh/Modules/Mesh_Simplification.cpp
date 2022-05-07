/*****************************************************************//**
 * \file   Mesh_Simplification.cpp
 * \brief  Mesh Simplification source code
 * 
 * \author Kevin Pruvost (pruvostkevin0@gmail.com)
 * \date   May, 06 2022
 *********************************************************************/
#include "Mesh_Simplification.hpp"

// Project includes
#include "OGL_Implementation\DebugInfo\Log.hpp"
#include "OGL_Implementation\DebugInfo\Timer.hpp"

// GLM includes
#include <glm/gtx/normal.hpp>

static Timer timer;

Mesh_Simplification::Mesh_Simplification()
{
}

Mesh_Simplification::~Mesh_Simplification()
{
}

std::vector<glm::mat4> Mesh_Simplification::GeneratePlanes(const std::vector<VertexPos> & vPs, const std::vector<Face> & originFaces)
{
    planes.clear();
    planes.reserve(originFaces.size());
    for (int i = 0; i < originFaces.size(); ++i)
    {
        const auto planeNormal = glm::vec4(glm::triangleNormal(
            vPs[originFaces[i].v[0]],
            vPs[originFaces[i].v[1]],
            vPs[originFaces[i].v[2]]), 1.0f);
        const float a = planeNormal.x, const b = planeNormal.y, const c = planeNormal.z;
        glm::outerProduct(planeNormal, planeNormal);
        glm::mat4 plane = {
            {a * a, a * b, a * c, a},
            {a * b, b * b, b * c, b},
            {a * c, b * c, c * c, c},
            {a    , b    , c    , 1.0f}
        };
        planes.push_back(plane);
//        planes.push_back(glm::outerProduct(planeNormal, planeNormal));
    }
    return planes;
}

void Mesh_Simplification::GenerateQMatrices(const std::vector<VertexPos> & vPs, const std::vector<Face> & originFaces)
{
    // Generating Q matrices
    qMatrices.clear();
    qMatrices.reserve(vPs.size());
    for (int i = 0; i < vPs.size(); ++i)
    {
        glm::mat4 & newQMatrix = qMatrices.emplace_back(1.0f);
        for (int j = 0; j < originFaces.size(); ++j)
        {
            if (originFaces[j].v[0] == i || originFaces[j].v[1] == i || originFaces[j].v[2] == i)
            {
                newQMatrix += planes[j];
            }
        }
    }
}

std::map<float, HalfEdge *> Mesh_Simplification::GenerateErrorMetrics(const std::vector<VertexPos> & vPs, const std::vector<Face> & originFaces, std::vector<std::unique_ptr<HalfEdge>> & halfEdges)
{
    std::map<float, HalfEdge *> errorMetrics;
    for (int i = 0; i < halfEdges.size(); ++i)
    {
        if (halfEdges[i]->pass)
        {
            halfEdges[i]->pass = false;
            continue;
        }
        float errors[4] = { 0.0f };
        const glm::vec4 vecs[4] = {
            { (vPs.at(halfEdges[i]->origin) + vPs.at(halfEdges[i]->next->origin)) / 2.0f, 1.0f },
            { vPs.at(halfEdges[i]->origin), 1.0f },
            { vPs.at(halfEdges[i]->next->origin), 1.0f },
            { glm::inverse((qMatrices[halfEdges[i]->origin] + qMatrices[halfEdges[i]->next->origin])) * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f) }
        };
        for (int j = 0; j < 4; ++j)
        {
            const glm::vec4 errorVec4 = (qMatrices[halfEdges[i]->origin] + qMatrices[halfEdges[i]->next->origin]) * vecs[j];
            const float error = glm::dot(errorVec4, vecs[j]);// errorVec4.x * vecs[j].x + errorVec4.y * vecs[j].y + errorVec4.z * vecs[j].z;
            errors[j] = abs(error);
        }
        errorMetrics.emplace(errors[0], halfEdges[i].get());
        if (halfEdges[i]->twin) halfEdges[i]->twin->pass = true; 
    }

    return errorMetrics;
}

Mesh_Custom * Mesh_Simplification::__Simplify(Mesh_Base & mesh)
{
    timer.Start();

    const std::vector<VertexPos> & vPs = *mesh.GetVerticesPos();
    const std::vector<Face> & originFaces = *mesh.GetFaces();
    std::vector<std::unique_ptr<HalfEdge>> halfEdges = GenerateHalfEdgesFromVertices(originFaces);

    GeneratePlanes(vPs, originFaces);
    GenerateQMatrices(vPs, originFaces);
    std::map<float, HalfEdge *> errorMetrics = GenerateErrorMetrics(vPs, originFaces, halfEdges);

    std::vector<Face> newFaces(originFaces);
    std::vector<VertexPos> newVertices(vPs);

    // Contraction
    int half = originFaces.size() / 2;
    for (int i = 0; i < 100; ++i)
    {
        auto part = errorMetrics.extract(errorMetrics.begin());
        //if (part.key() >= 0.1f) break;
        HalfEdge * edge = part.mapped();
        int vid1 = edge->origin;
        int vid2 = edge->next->origin;

        if (vid2 < vid1) std::swap(vid1, vid2);
        //LOG_PRINT(stdout, "VIDS: %d %d\n [%.2f, %.2f, %.2f]\n", vid1, vid2, newVertices[vid2].x - newVertices[vid1].x, newVertices[vid2].y - newVertices[vid1].y, newVertices[vid2].z - newVertices[vid1].z);

        // Assign contracted point to v1, to later remove v2
        VertexPos contractedPoint = (newVertices[vid1] + newVertices[vid2]) / 2.0f;
        newVertices[vid1] = contractedPoint;

        std::vector<int> facesRemoved;
        for (int j = 0; j < newFaces.size(); ++j)
        {
            std::vector<int>::iterator ite;
            if (std::find(newFaces[j].v.begin(), newFaces[j].v.end(), vid1) != newFaces[j].v.end()
             && std::find(newFaces[j].v.begin(), newFaces[j].v.end(), vid2) != newFaces[j].v.end())
            {
                // If face contains the 2 vertices contracted
                facesRemoved.push_back(j);
                qMatrices[newFaces[j].v[0]] -= planes[j];
                qMatrices[newFaces[j].v[1]] -= planes[j];
                qMatrices[newFaces[j].v[2]] -= planes[j];
            }
            else if ((ite = std::find(newFaces[j].v.begin(), newFaces[j].v.end(), vid2)) != newFaces[j].v.end())
            {
                //LOG_PRINT(stdout, "Face %d modified\n", j);
                // If face contains v2, then reassign to v1
                qMatrices[newFaces[j].v[0]] -= planes[j];
                qMatrices[newFaces[j].v[1]] -= planes[j];
                qMatrices[newFaces[j].v[2]] -= planes[j];
                *ite = vid1;
                // Update Plane
                // const auto planeNormal = glm::triangleNormal(
                //     newVertices[newFaces[j].v[0]],
                //     newVertices[newFaces[j].v[1]],
                //     newVertices[newFaces[j].v[2]]);
                const auto planeNormal = glm::vec4(glm::triangleNormal(
                    vPs[originFaces[i].v[0]],
                    vPs[originFaces[i].v[1]],
                    vPs[originFaces[i].v[2]]), 1.0f);
                const float a = planeNormal.x, const b = planeNormal.y, const c = planeNormal.z;
                glm::outerProduct(planeNormal, planeNormal);
                glm::mat4 plane = {
                    {a * a, a * b, a * c, a},
                    {a * b, b * b, b * c, b},
                    {a * c, b * c, c * c, c},
                    {a    , b    , c    , 1.0f}
                };
                //const glm::mat4 plane = glm::outerProduct(planeNormal, planeNormal);
                
                planes[j] = plane;
                qMatrices[newFaces[j].v[0]] += planes[j];
                qMatrices[newFaces[j].v[1]] += planes[j];
                qMatrices[newFaces[j].v[2]] += planes[j];
                for (int x = 0; x < 3; ++x)
                {
                    if (newFaces[j].v[x] > vid2) --newFaces[j].v[x];
                }
            }
            else
            {
                for (int x = 0; x < 3; ++x)
                {
                    if (newFaces[j].v[x] > vid2) --newFaces[j].v[x];
                }
            }
        }

        for (int x = 0; x < halfEdges.size(); ++x)
        {
            if (std::find(facesRemoved.begin(), facesRemoved.end(), halfEdges[x]->face) != facesRemoved.end())
            {
                for (auto & ite : errorMetrics)
                {
                    if (ite.second == halfEdges[x].get())
                    {
                        errorMetrics.erase(ite.first);
                        break;
                    }
                }
                halfEdges.erase(halfEdges.begin() + x--);
                continue;
            }
            else if (halfEdges[x]->origin == vid2) halfEdges[x]->origin = vid1;
            else if (halfEdges[x]->origin > vid2) --halfEdges[x]->origin;

            for (int p = facesRemoved.size() - 1; p >= 0; --p)
            {
                if (halfEdges[x]->face > facesRemoved[p])
                    --halfEdges[x]->face;
            }
        }

        for (int i = 0; i < facesRemoved.size(); ++i)
        {
            newFaces.erase(newFaces.begin() + (facesRemoved[i] - i));
            // Update Planes
            planes.erase(planes.begin() + (facesRemoved[i] - i));
        }
        newVertices.erase(newVertices.begin() + vid2);
        qMatrices.erase(qMatrices.begin() + vid2);

        //GeneratePlanes(newVertices, newFaces);
        //GenerateQMatrices(newVertices, newFaces);
        errorMetrics = GenerateErrorMetrics(newVertices, newFaces, halfEdges);
    }

    LOG_PRINT(stdout, "Final Time: %.2fms\n", timer.GetMsTime());

    Mesh_Custom * newMesh = new Mesh_Custom(newVertices, newFaces);
    newMesh->GenerateNormals(false);
    return newMesh;
}
