/*****************************************************************//**
 * \file   Obj.hpp
 * \brief  .obj files loading class/parser
 * 
 * \author Kevin Pruvost (pruvostkevin0@gmail.com)
 * \date   March, 26 2022
 *********************************************************************/
#pragma once

// C++ includes
#include <exception>
#include <iostream>
#include <sstream>
#include <vector>
#include <array>

// Glad includes
#include <glad/glad.h>

#include "Mesh/Mesh_Geometry.hpp"

/**
 * @brief Manages parsing and loading of .obj files.
*/
class Obj
{
public:
	Obj();

	std::vector<glm::vec3> verticesPos;
	std::vector<glm::vec2> verticesTextureCoordinates;
	std::vector<glm::vec3> verticesNormals;
	std::vector<std::string> materialNames;
	std::vector<Face> faces;

	/**
	 * @brief Returns Count of triangle faces.
	 * @return count of triangle faces
	*/
	int numTriangles() const { return int(faces.size()); }
	/**
	 * @brief Returns Count of vertices.
	 * @return count of vertices
	*/
	int numVertices() const { return int(verticesPos.size()); }
	
	/**
	 * @brief Generates Normals if there is none
	 * @param smooth 
	*/
	void GenerateNormals(bool smooth = true);

	/**
	 * @brief Tries loading of a .obj file
	 * @param fileName
	 * @return true if everything happened without errors, false otherwise
	*/
	bool TryLoad(const char * fileName);
};