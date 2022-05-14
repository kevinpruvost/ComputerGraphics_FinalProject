/*****************************************************************//**
 * \file   Mesh_ThreadPool.hpp
 * \brief  Mesh Thread Pool
 * 
 * \author Kevin Pruvost (pruvostkevin0@gmail.com)
 * \date   May, 12 2022
 *********************************************************************/
#pragma once

// Project includes
#include "../Mesh_Base.hpp"

// C++ includes
#include <thread>
#include <mutex>
#include <unordered_map>
#include <functional>
#include <condition_variable>

class Mesh_Simplification;
class Mesh_Subdivision;

class Mesh_Thread;

/**
 * @brief Thread Pool for multithreaded operations on meshes, only accessible to Mesh operations classes
*/
class Mesh_ThreadPool
{
    friend class Mesh_Simplification;
    friend class Mesh_Subdivision;

private:
    static void SetNewThread(Mesh_Base * mesh, const std::function<void(Mesh_Base *, bool *, bool *, bool *, bool *, std::condition_variable *, std::mutex *)> & threadLambda);

public:
    static bool Refresh(Mesh_Base * mesh);
};

/**
 * @brief Automate Mesh's threads construction & destruction primarily
*/
class Mesh_Thread
{
    friend class Mesh_ThreadPool;

private:
    Mesh_Thread(Mesh_Base * mesh, const std::function<void(Mesh_Base *, bool *, bool *, bool *, bool *, std::condition_variable *, std::mutex *)> & threadLambda);

public:
    ~Mesh_Thread();

public:
    bool __threadFinished;
    bool __threadLoopFinished;
    bool __threadMainWorking;
    bool __threadAbort;
    std::condition_variable __threadMainWorkingCV;
    std::mutex __threadMutex;
    std::thread __thread;
};
