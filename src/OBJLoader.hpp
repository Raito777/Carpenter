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
            std::vector<std::string> faceTokens;
            std::string              token;
            while (ss >> token)
            {
                // pushing indices into correct arrays
                faceTokens.push_back(token);
            }
            // split each token into its constituent parts
            std::vector<std::vector<std::string>> vertexTokens(faceTokens.size());
            for (size_t i = 0; i < faceTokens.size(); i++)
            {
                std::istringstream vertexIss(faceTokens[i]);
                std::string        indexToken;
                while (std::getline(vertexIss, indexToken, '/'))
                {
                    vertexTokens[i].push_back(indexToken);
                }
            }

            // generate triangles for the face
            if (vertexTokens.size() == 3) // triangle
            {
                for (size_t i = 0; i < vertexTokens.size(); i++)
                {
                    GLuint positionIndex = std::stoi(vertexTokens[i][0]) - 1;
                    GLuint texcoordIndex = 0;
                    if (vertexTokens[i].size() >= 2 && !vertexTokens[i][1].empty())
                    {
                        texcoordIndex = std::stoi(vertexTokens[i][1]) - 1;
                    }
                    GLuint normalIndex = 0;
                    if (vertexTokens[i].size() >= 3 && !vertexTokens[i][2].empty())
                    {
                        normalIndex = std::stoi(vertexTokens[i][2]) - 1;
                    }
                    vertexPositionIndicies.push_back(positionIndex);
                    vertexTexCoordIndicies.push_back(texcoordIndex);
                    vertexNormalIndicies.push_back(normalIndex);
                }
            }
            else if (vertexTokens.size() == 4) // quad
            {
                GLuint indicesArray[6] = {0, 1, 2, 2, 3, 0}; // triangle strip to draw quad
                for (size_t i = 0; i < 6; i++)
                {
                    GLuint index         = std::stoi(vertexTokens[indicesArray[i]][0]) - 1;
                    GLuint texcoordIndex = 0;
                    if (vertexTokens[indicesArray[i]].size() >= 2 && !vertexTokens[indicesArray[i]][1].empty())
                    {
                        texcoordIndex = std::stoi(vertexTokens[indicesArray[i]][1]) - 1;
                    }
                    GLuint normalIndex = 0;
                    if (vertexTokens[indicesArray[i]].size() >= 3 && !vertexTokens[indicesArray[i]][2].empty())
                    {
                        normalIndex = std::stoi(vertexTokens[indicesArray[i]][2]) - 1;
                    }
                    vertexPositionIndicies.push_back(index);
                    vertexTexCoordIndicies.push_back(texcoordIndex);
                    vertexNormalIndicies.push_back(normalIndex);
                }
            }
            else
            {
                std::cerr << "Face avec un nombre d'indices invalide" << std::endl;
            }
        }

        else
        {
        }
    }
    // build final shapeVertex;
    vertices.resize(vertexPositionIndicies.size(), glimac::ShapeVertex());

    // for (size_t i = 0; i < vertexPositions.size(); i++)
    // {
    //     std::cout << vertexPositions[i].x << " " << vertexPositions[i].y << " " << vertexPositions[i].z << "\n";
    // }
    // for (size_t i = 0; i < vertexPositionIndicies.size(); i++)
    // {
    //     std::cout << vertexPositionIndicies[i] << "\n";
    // }
    for (size_t i = 0; i < vertices.size(); i++)
    {
        vertices[i].position  = vertexPositions[vertexPositionIndicies[i]];
        vertices[i].normal    = vertexNormals[vertexNormalIndicies[i]];
        vertices[i].texCoords = vertexTexCoords[vertexTexCoordIndicies[i]];
        // std::cout << vertexPositionIndicies[i] << "\n";
    }

    std::cout << "Nb of vertices: " << vertices.size() << "\n";
    std::cout << "Nb of indicies pos : " << vertexPositionIndicies.size() << "\n";
    std::cout << "Nb of indicies normals : " << vertexNormalIndicies.size() << "\n";

    std::cout << "OBJ file loaded!"
              << "\n";

    // Loaded succes
    return vertices;
}