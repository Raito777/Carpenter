#pragma once

#include <stdlib.h>
#include <random>
#include <vector>
#include "glm/fwd.hpp"
#include "glm/geometric.hpp"
#include "p6/p6.h"

class Boid {
private:
public:
    glm::vec3 m_position;

    Boid()
        : m_position({0, 0, -5}){};
    Boid(const glm::vec3& position)
        : m_position(position){};
};