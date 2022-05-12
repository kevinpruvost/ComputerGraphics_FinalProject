/*****************************************************************//**
 * \file   Mesh_ThreadPool.cpp
 * \brief  Mesh ThreadPool source code
 * 
 * \author Kevin Pruvost (pruvostkevin0@gmail.com)
 * \date   May, 12 2022
 *********************************************************************/
#include "Mesh_ThreadPool.hpp"

static std::unordered_map<Mesh_Base *, std::unique_ptr<std::thread>> __threads;
static std::unordered_map<Mesh_Base *, std::unique_ptr<bool>> __threadsFinished;
static std::unordered_map<Mesh_Base *, std::unique_ptr<bool>> __threadsLoopFinished;
static std::unordered_map<Mesh_Base *, std::unique_ptr<std::mutex>> __threadsMutex;

void Mesh_ThreadPool::SetNewThread(Mesh_Base * mesh, const std::function<void(Mesh_Base *, bool *, bool *, std::mutex *)> & threadLambda)
{
    __threadsFinished.emplace(mesh, new bool(false));
    __threadsLoopFinished.emplace(mesh, new bool(false));
    __threadsMutex.emplace(mesh, new std::mutex());
    auto finished = __threadsFinished[mesh].get();
    auto loopFinished = __threadsLoopFinished[mesh].get();
    auto mutex = __threadsMutex[mesh].get();
    __threads.emplace(mesh, new std::thread(threadLambda, mesh, finished, loopFinished, mutex));
}

bool Mesh_ThreadPool::Refresh(Mesh_Base * mesh)
{
    if (!__threads.contains(mesh)) return true;

    if (*__threadsFinished[mesh] || *__threadsLoopFinished[mesh])
    {
        __threadsMutex[mesh]->lock();
        mesh->GenerateNormals(true);
        mesh->UpdateVerticesToApi();
        __threadsMutex[mesh]->unlock();
    }
    *__threadsLoopFinished[mesh] = false;
    if (*__threadsFinished[mesh])
    {
        __threads[mesh]->join();
        __threads.erase(mesh);
        __threadsFinished.erase(mesh);
        __threadsLoopFinished.erase(mesh);
        __threadsMutex.erase(mesh);
        return true;
    }
    return false;
}
