/*****************************************************************//**
 * \file   Mesh_ThreadPool.cpp
 * \brief  Mesh ThreadPool source code
 * 
 * \author Kevin Pruvost (pruvostkevin0@gmail.com)
 * \date   May, 12 2022
 *********************************************************************/
#include "Mesh_ThreadPool.hpp"

#include "OGL_Implementation\DebugInfo\Log.hpp"

static std::unordered_map<Mesh_Base *, std::unique_ptr<Mesh_Thread>> threads;

void Mesh_ThreadPool::SetNewThread(Mesh_Base * mesh, const std::function<void(Mesh_Base *, bool *, bool *, bool *, bool *, std::condition_variable *, std::mutex *)> & threadLambda)
{
    threads.emplace(mesh, new Mesh_Thread(mesh, threadLambda));
}

bool Mesh_ThreadPool::Refresh(Mesh_Base * mesh)
{
    if (!threads.contains(mesh)) return true;

    Mesh_Thread * thread = threads.at(mesh).get();

    if (thread->__threadFinished || thread->__threadLoopFinished)
    {
        if (thread->__threadMainWorking)
        {
            std::unique_lock<std::mutex> lk(thread->__threadMutex);
            thread->__threadMainWorkingCV.wait(lk, [&] {return !thread->__threadMainWorking; });
        }
        while (!thread->__threadMutex.try_lock());
        thread->__threadMainWorking = true;
        mesh->GenerateNormals(true);
        mesh->UpdateVerticesToApi();
        thread->__threadMainWorking = false;
        thread->__threadMainWorkingCV.notify_all();
        thread->__threadLoopFinished = false;
        thread->__threadMutex.unlock();
        if (thread->__threadFinished)
        {
            threads.erase(mesh);
            return true;
        }
    }
    return false;
}

Mesh_Thread::Mesh_Thread(Mesh_Base * mesh, const std::function<void(Mesh_Base *, bool *, bool *, bool *, bool *, std::condition_variable *, std::mutex *)> & threadLambda)
    : __threadFinished{ false }
    , __threadLoopFinished{ false }
    , __threadMainWorking{ false }
    , __threadMainWorkingCV{}
    , __threadAbort{ false }
    , __threadMutex{}
    , __thread(threadLambda, mesh, &__threadFinished, &__threadLoopFinished, &__threadMainWorking, &__threadAbort, &__threadMainWorkingCV, &__threadMutex)
{
}

Mesh_Thread::~Mesh_Thread()
{
    __threadAbort = true;
    __thread.join();
}
