#pragma once

#include <p6/p6.h>
#include <stddef.h>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include "glimac/common.hpp"
#include "glm/ext.hpp"
#include "glm/fwd.hpp"

static std::vector<glimac::ShapeVertex> loadOBJ(const char* fileName)
{
    // Vertex portions
    std::vector<glm::fvec3> vertexPositions;
    std::vector<glm::fvec2> vertexTexCoords;
    std::vector<glm::fvec3> vertexNormals;

    std::vector<GLint> vertexPositionIndicies;
    std::vector<GLint> vertexNormalIndicies;
    std::vector<GLint> vertexTexCoordIndicies;

    std::vector<glimac::ShapeVertex> vertices;

    std::stringstream ss;
    std::ifstream     inFile(fileName);
    std::string       line   = "";
    std::string       prefix = "";
    glm::fvec3        tempVec3;
    glm::fvec2        tempVec2;
    GLint             tempGLint;

    // file open error check
    if (!inFile.is_open())
    {
        std::cout << "ERROR::OBJLOADER::Could not open file."
                  << "\n";
        throw "ERROR::OBJLOADER::Could not open file.";
    }
    // read line per line
    while (std::getline(inFile, line))
    {
        // prefix of the line
        ss.clear();
        ss.str(line);
        ss >> prefix;

        if (prefix == "#")
        {
        }

        else if (prefix == "o")
        {
        }

        else if (prefix == "s")
        {
        }

        else if (prefix == "usemtl")
        {
        }

        else if (prefix == "g")
        {
        }

        else if (prefix == "v") // vertex position
        {
            ss >> tempVec3.x >> tempVec3.y >> tempVec3.z;
            vertexPositions.push_back(tempVec3);
        }

        else if (prefix == "vt")
        {
            ss >> tempVec2.x >> tempVec2.y;
            vertexTexCoords.push_back(tempVec2);
        }

        else if (prefix == "vn") // vertex position
        {
            ss >> tempVec3.x >> tempVec3.y >> tempVec3.z;
            vertexNormals.push_back(tempVec3);
        }

        else if (prefix == "f")
        {
            int counter = 0;
            while (ss >> tempGLint)
            {
                // pushing indecies into correct arrays

                if (counter == 0)
                {
                    vertexPositionIndicies.push_back(tempGLint);
                }
                else if (counter == 1)
                {
                    vertexTexCoordIndicies.push_back(tempGLint);
                }
                else if (counter == 2)
                {
                    vertexNormalIndicies.push_back(tempGLint);
                }
                // andling characters;
                if (ss.peek() == '/')
                {
                    counter++;
                    ss.ignore(1, '/');
                }
                else if (ss.peek() == ' ')
                {
                    counter++;
                    ss.ignore(1, ' ');
                }
                if (counter > 2)
                {
                    counter = 0;
                }
            }
        }

        else
        {
        }
    }
    // build final shapeVertex;
    vertices.resize(vertexPositionIndicies.size(), glimac::ShapeVertex());

    for (size_t i = 0; i < vertexPositions.size(); i++)
    {
        std::cout << vertexPositions[i].x << " " << vertexPositions[i].y << " " << vertexPositions[i].z << "\n";
    }
    for (size_t i = 0; i < vertexPositionIndicies.size(); i++)
    {
        std::cout << vertexPositionIndicies[i] << "\n";
    }
    for (size_t i = 0; i < vertices.size(); i++)
    {
        vertices[i].position = vertexPositions[vertexPositionIndicies[i] - 1];
        // vertices[i].normal    = vertexNormals[vertexNormalIndicies[i] - 1];
        // vertices[i].texCoords = vertexTexCoords[vertexTexCoordIndicies[i] - 1];
        // std::cout << vertexPositionIndicies[i] << "\n";
    }

    std::cout << "Nb of vertices: " << vertexNormals.size() << "\n";
    std::cout << "Nb of indicies pos : " << vertexNormalIndicies.size() << "\n";
    std::cout << "Nb of indicies normals : " << vertexNormalIndicies.size() << "\n";

    std::cout << "OBJ file loaded!"
              << "\n";

    // Loaded succes
    return vertices;
}