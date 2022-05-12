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

class Mesh_Simplification;
class Mesh_Subdivision;

/**
 * @brief Thread Pool for multithreaded operations on meshes, only accessible to Mesh operations classes
*/
class Mesh_ThreadPool
{
    friend class Mesh_Simplification;
    friend class Mesh_Subdivision;

private:
    static void SetNewThread(Mesh_Base * mesh, const std::function<void(Mesh_Base *, bool *, bool *, std::mutex *)> & threadLambda);

public:
    static bool Refresh(Mesh_Base * mesh);
};
