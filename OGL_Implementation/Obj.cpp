/*****************************************************************//**
 * \file   Obj.cpp
 * \brief  Source code of Obj class
 * 
 * \author Kevin Pruvost (pruvostkevin0@gmail.com)
 * \date   March, 26 2022
 *********************************************************************/
#include "Obj.hpp"

// C++ includes
#include <fstream>
#include <algorithm>
#include <chrono>

// Project includes
#include "OGL_Implementation\DebugInfo\Log.hpp"

constexpr const std::array<std::array<float, 3>, 4> colorsRand = { {
	{0.03f, 0.67f, 0.207f}, // Dark Green
	{0.498f, 0.105f, 0.8901f}, // Clear Purple
	{0.176f, 0.564f, 0.929f}, // Cyanish
	{0.909f, 0.745f, 0.2901f} // Weird Yellow
} };

static std::vector<std::string> tokenize(std::string const & str, const char * const delim)
{
	std::vector<std::string> out;
	size_t start;
	size_t end = 0;

	while ((start = str.find_first_not_of(delim, end)) != std::string::npos)
	{
		end = str.find(delim, start);
		out.push_back(str.substr(start, end - start));
	}
	return out;
}

Obj::Obj()
{
}

void Obj::GenerateNormals(bool smooth)
{
	verticesNormals.clear();
	verticesNormals.resize(verticesPos.size());

	for (auto & face : faces)
	{
		face.vn = face.v;
		const VertexPos & p0 = verticesPos[face.v[0]], & p1 = verticesPos[face.v[1]], & p2 = verticesPos[face.v[2]];
		glm::vec3 faceNormal = glm::cross(p0 - p1, p1 - p2);

		if (smooth)
		{
			verticesNormals[face.vn[0]] += faceNormal;
			verticesNormals[face.vn[1]] += faceNormal;
			verticesNormals[face.vn[2]] += faceNormal;
		}
		else
		{
			verticesNormals[face.vn[0]] = faceNormal;
			verticesNormals[face.vn[1]] = faceNormal;
			verticesNormals[face.vn[2]] = faceNormal;
		}
	}
}

bool Obj::TryLoad(const char * fileName)
{
	std::ifstream in;
	// ensures ifstream objects can throw exceptions:
	in.exceptions(std::ifstream::badbit);
	in.open(fileName, std::ios::in);
	if (!in.is_open())
		return false;

	std::string buf;
	std::string cmd;
	in.exceptions(std::istream::badbit);
	
	while (!in.eof())
	{
		try
		{
			std::getline(in, buf);
			std::istringstream iss(buf);
			cmd.clear();
			iss >> cmd;
	
			if (cmd.size() == 0 || cmd[0] == '#' || cmd[0] == '\n')
			{
				continue;
			}
			// Geometric Vertex
			else if (cmd.compare("v") == 0)
			{
				GLfloat x, y, z;
				iss >> x >> y >> z;
				verticesPos.emplace_back(x, y, z);
			}
			// Face
			else if (cmd.compare("f") == 0)
			{
				std::string f;
				std::getline(iss, f, '\n');
				std::stringstream fStream(f);

				std::vector<int> v, vt, vn;

				while (!fStream.eof())
				{
					std::string facePart;
					fStream >> facePart;
					// Only Vertex + Normals
					if (facePart.find("//") != std::string::npos)
					{
						std::vector<std::string> parts = tokenize(facePart, "//");
						
						v.emplace_back(std::stoi(parts[0]) - 1);
						vn.emplace_back(std::stoi(parts[1]) - 1);
					}
					else if (facePart.find("/") != std::string::npos)
					{
						std::vector<std::string> parts = tokenize(facePart, "/");

						switch (parts.size())
						{
							// V / VT / VN
							case 3:
								v.emplace_back(std::stoi(parts[0]) - 1);
								vt.emplace_back(std::stoi(parts[1]) - 1);
								vn.emplace_back(std::stoi(parts[2]) - 1);
								break;
							// V / VT
							case 2:
								v.emplace_back(std::stoi(parts[0]) - 1);
								vt.emplace_back(std::stoi(parts[1]) - 1);
								break;
							// V
							case 1:
								v.emplace_back(std::stoi(parts[0]) - 1);
								break;
						}
					}
					else
					{
						v.emplace_back(std::stoi(facePart) - 1);
					}
				}
				faces.emplace_back(v, vt, vn);
			}
			// Texture Coordinate (inversed y)
			else if (cmd.compare("vt") == 0)
			{
				GLfloat x, y;
				iss >> x >> y;
				verticesTextureCoordinates.emplace_back(x, 1.0f - y);
			}
			// Vertex Normals
			else if (cmd.compare("vn") == 0)
			{
				GLfloat x, y, z;
				iss >> x >> y >> z;
				verticesNormals.emplace_back(x, y, z);
			}
			// Space Vertices
			else if (cmd.compare("vp") == 0)
			{
			}
			// Material File used
			else if (cmd.compare("mtllib") == 0)
			{
				std::string path;
				iss >> path;
				materialNames.emplace_back();
			}
			// Material Name used
			else if (cmd.compare("usemtl") == 0)
			{
				std::string path;
				iss >> path;
				materialNames.emplace_back();
			}
			// Smooth Shading (http://paulbourke.net/dataformats/obj/)
			else if (cmd.compare("s") == 0)
			{

			}
			// Object group (not important)
			else if (cmd.compare("o") == 0)
			{

			}
			// Line
			else if (cmd.compare("l") == 0)
			{

			}
			else
			{
				std::cerr << "Warning: unsupported line type starting with '" << buf[0] << "'" << std::endl;
				return false;
			}
		} catch (const std::exception & e)
		{
			std::cerr << "Reading obj file error: " << e.what() << std::endl;
			return false;
		}
	}

	return true;
}