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

// C++ includes
#include <chrono>
using namespace std::chrono_literals;

void Mesh_Simplification::GeneratePlanes(const std::vector<VertexPos> & vPs, const std::vector<Face> & originFaces, std::vector<glm::mat4> & planes)
{
    planes.clear();
    planes.reserve(originFaces.size());
    for (int i = 0; i < originFaces.size(); ++i)
    {
        glm::vec4 planeNormal = GetPlaneEquationFromTriangle(
            vPs[originFaces[i].v[0]],
            vPs[originFaces[i].v[1]],
            vPs[originFaces[i].v[2]]);
        planes.push_back(glm::outerProduct(planeNormal, planeNormal));
    }
}

void Mesh_Simplification::GenerateQMatrices(const std::vector<VertexPos> & vPs, const std::vector<Face> & originFaces, const std::vector<glm::mat4> & planes, std::vector<glm::mat4> & qMatrices)
{
    // Generating Q matrices
    qMatrices.clear();
    qMatrices.resize(vPs.size(), glm::mat4(0.0f));
    for (int j = 0; j < originFaces.size(); ++j)
    {
        qMatrices[originFaces[j].v[0]] += planes[j];
        qMatrices[originFaces[j].v[1]] += planes[j];
        qMatrices[originFaces[j].v[2]] += planes[j];
    }
}

std::map<float, HalfEdge *> Mesh_Simplification::GenerateErrorMetrics(const std::vector<VertexPos> & vPs, const std::vector<Face> & originFaces, std::vector<std::unique_ptr<HalfEdge>> & halfEdges, const std::vector<glm::mat4> & qMatrices)
{
    std::map<float, HalfEdge *> errorMetrics;
    for (int i = 0; i < halfEdges.size(); ++i)
    {
        if (halfEdges[i]->pass)
        {
            halfEdges[i]->pass = false;
            continue;
        }
        /*float errors[4] = { 0.0f };
        const glm::vec4 vecs[4] = {
            { (vPs.at(halfEdges[i]->origin) + vPs.at(halfEdges[i]->next->origin)) / 2.0f, 1.0f },
            { vPs.at(halfEdges[i]->origin), 1.0f },
            { vPs.at(halfEdges[i]->next->origin), 1.0f },
            { glm::inverse((qMatrices[halfEdges[i]->origin] + qMatrices[halfEdges[i]->next->origin])) * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f) }
        };
        for (int j = 0; j < 4; ++j)
        {
            const glm::vec4 errorVec4 = (qMatrices[halfEdges[i]->origin] + qMatrices[halfEdges[i]->next->origin]) * vecs[j];
            const float error = glm::dot(errorVec4, vecs[j]);
            errors[j] = abs(error);
        }*/
        const glm::vec4 contractPoint = { (vPs.at(halfEdges[i]->origin) + vPs.at(halfEdges[i]->next->origin)) / 2.0f, 1.0f };
        const glm::vec4 errorVec4 = contractPoint * (qMatrices[halfEdges[i]->origin] + qMatrices[halfEdges[i]->next->origin]);
        const float error = glm::dot(errorVec4, contractPoint);
        errorMetrics.emplace(abs(error), halfEdges[i].get());
        if (halfEdges[i]->twin) halfEdges[i]->twin->pass = true; 
    }

    return errorMetrics;
}

void Mesh_Simplification::SimplifyParallel(Mesh_Base & mesh)
{
    const auto threadFunc = [](Mesh_Base * mesh, bool * finished, bool * loopFinished, bool * working, bool * abort, std::condition_variable * cv, std::mutex * mutex) {
        Timer timer;
        if (mesh->GetVerticesCount() == 0) return;

        const std::vector<VertexPos> & vPs = *mesh->GetVerticesPos();
        const std::vector<VertexTextureCoordinates> & vTs = *mesh->GetVerticesTextureCoordinates();
        const std::vector<Face> & originFaces = *mesh->GetFaces();
        std::vector<std::unique_ptr<HalfEdge>> halfEdges = GenerateHalfEdgesFromVertices(originFaces);

        std::vector<glm::mat4> qMatrices;
        std::vector<glm::mat4> planes;
        GeneratePlanes(vPs, originFaces, planes);
        GenerateQMatrices(vPs, originFaces, planes, qMatrices);
        std::map<float, HalfEdge *> errorMetrics = GenerateErrorMetrics(vPs, originFaces, halfEdges, qMatrices);

        std::vector<Face> newFaces(originFaces);
        std::vector<VertexPos> newVertices(vPs);
        std::vector<VertexTextureCoordinates> newTextureCoords(vTs);

        // Contraction
        size_t initFacesCount = newFaces.size() / 2;
        while (newFaces.size() > initFacesCount)
        {
            auto part = errorMetrics.extract(errorMetrics.begin());
            //if (part.key() >= 0.1f) break;
            HalfEdge * edge = part.mapped();
            int vid1 = edge->origin;
            int vid2 = edge->next->origin;

            if (vid2 < vid1) std::swap(vid1, vid2);

            // Assign contracted point to v1, to later remove v2
            VertexPos contractedPoint = (newVertices[vid1] + newVertices[vid2]) / 2.0f;
            newVertices[vid1] = contractedPoint;
            VertexTextureCoordinates contractedTextureCoords(0.0f);

            std::vector<int> facesRemoved;
            for (int j = 0; j < newFaces.size(); ++j)
            {
                std::vector<int>::iterator ite;
                if (std::find(newFaces[j].v.begin(), newFaces[j].v.end(), vid1) != newFaces[j].v.end()
                    && std::find(newFaces[j].v.begin(), newFaces[j].v.end(), vid2) != newFaces[j].v.end())
                {
                    if (contractedTextureCoords == glm::vec2(0.0f) && !newFaces[j].vt.empty())
                    {
                        for (int k = 0; k < 3; ++k)
                        {
                            if (newFaces[j].v[k] == vid1 || newFaces[j].v[k] == vid2)
                                contractedTextureCoords += newTextureCoords[newFaces[j].vt[k]];
                        }
                        contractedTextureCoords *= 0.5f;
                    }
                    // If face contains the 2 vertices contracted
                    facesRemoved.push_back(j);
                    qMatrices[newFaces[j].v[0]] -= planes[j];
                    qMatrices[newFaces[j].v[1]] -= planes[j];
                    qMatrices[newFaces[j].v[2]] -= planes[j];
                    continue;
                }
                else if ((ite = std::find(newFaces[j].v.begin(), newFaces[j].v.end(), vid2)) != newFaces[j].v.end())
                {
                    // If face contains v2, then reassign to v1
                    *ite = vid1;
                    // Update Plane
                    qMatrices[newFaces[j].v[0]] -= planes[j];
                    qMatrices[newFaces[j].v[1]] -= planes[j];
                    qMatrices[newFaces[j].v[2]] -= planes[j];
                    const glm::vec4 planeNormal = GetPlaneEquationFromTriangle(
                        newVertices[newFaces[j].v[0]],
                        newVertices[newFaces[j].v[1]],
                        newVertices[newFaces[j].v[2]]);
                    planes[j] = glm::outerProduct(planeNormal, planeNormal);
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
                if (!newFaces[j].vt.empty())
                {
                    for (int k = 0; k < 3; ++k)
                    {
                        if (newFaces[j].v[k] == vid1)
                            newFaces[j].vt[k] = newTextureCoords.size();
                    }
                }
            }
            if (!newTextureCoords.empty()) newTextureCoords.push_back(contractedTextureCoords);

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

            errorMetrics = GenerateErrorMetrics(newVertices, newFaces, halfEdges, qMatrices);
            if (*abort) return;
            if (*working)
            {
                std::unique_lock<std::mutex> lk(*mutex);
                cv->wait_for(lk, 3s, [&] { return !*working; });
            }
            *working = true;
            mutex->lock();
            mesh->SetGeometry(newFaces, newVertices, {}, newTextureCoords);
            mutex->unlock();
            *loopFinished = true;
            *working = false;
            cv->notify_all();
        }
        Log::Print(Log::LogMainFileName, "Final Time Simplification: %.2fms\n", timer.GetMsTime());
        LOG_PRINT(stdout, "Final Time: %.2fms\n", timer.GetMsTime());
        *finished = true;
    };
    Mesh_ThreadPool::SetNewThread(&mesh, threadFunc);
}

Mesh_Custom * Mesh_Simplification::Simplify(Mesh_Base & mesh)
{
    DEBUG_CODE(static Timer timer);
    DEBUG_CODE(timer.Start());

    if (mesh.GetVerticesCount() == 0) return nullptr;

    const std::vector<VertexPos> & vPs = *mesh.GetVerticesPos();
    const std::vector<Face> & originFaces = *mesh.GetFaces();
    std::vector<std::unique_ptr<HalfEdge>> halfEdges = GenerateHalfEdgesFromVertices(originFaces);

    std::vector<glm::mat4> qMatrices;
    std::vector<glm::mat4> planes;
    GeneratePlanes(vPs, originFaces, planes);
    GenerateQMatrices(vPs, originFaces, planes, qMatrices);
    std::map<float, HalfEdge *> errorMetrics = GenerateErrorMetrics(vPs, originFaces, halfEdges, qMatrices);

    std::vector<Face> newFaces(originFaces);
    std::vector<VertexPos> newVertices(vPs);

    // Contraction
    size_t initFacesCount = newFaces.size() / 2;
    while (newFaces.size() > initFacesCount)
    {
        auto part = errorMetrics.extract(errorMetrics.begin());
        //if (part.key() >= 0.1f) break;
        HalfEdge * edge = part.mapped();
        int vid1 = edge->origin;
        int vid2 = edge->next->origin;

        if (vid2 < vid1) std::swap(vid1, vid2);

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
                // If face contains v2, then reassign to v1
                qMatrices[newFaces[j].v[0]] -= planes[j];
                qMatrices[newFaces[j].v[1]] -= planes[j];
                qMatrices[newFaces[j].v[2]] -= planes[j];
                *ite = vid1;
                // Update Plane
                const glm::vec4 planeNormal = GetPlaneEquationFromTriangle(
                    newVertices[newFaces[j].v[0]],
                    newVertices[newFaces[j].v[1]],
                    newVertices[newFaces[j].v[2]]);                
                planes[j] = glm::outerProduct(planeNormal, planeNormal);
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

        errorMetrics = GenerateErrorMetrics(newVertices, newFaces, halfEdges, qMatrices);
    }

    LOG_PRINT(stdout, "Final Time: %.2fms\n", timer.GetMsTime());

    Mesh_Custom * newMesh = new Mesh_Custom(newVertices, newFaces);
    newMesh->GenerateNormals(false);
    return newMesh;
}
