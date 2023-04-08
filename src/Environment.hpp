#pragma once

#include <stdlib.h>
#include <random>
#include <vector>
#include "glm/fwd.hpp"
#include "glm/geometric.hpp"
#include "p6/p6.h"

class Environment {
private:
public:
    float m_sizeX = 3.f;
    float m_sizeY = 1.5f;
    float m_sizeZ = 3.f;

    glm::vec3 m_position = glm::vec3(0, 0, 0);

    Environment(){};
    Environment(float sizeX, float sizeY, float sizeZ)
        : m_sizeX(sizeX), m_sizeY(sizeY), m_sizeZ(sizeZ){};
};