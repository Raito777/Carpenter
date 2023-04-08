#pragma once

#include <stdlib.h>
#include <random>
#include <vector>
#include "Environment.hpp"
#include "glm/fwd.hpp"
#include "glm/geometric.hpp"
#include "p6/p6.h"

class Boid {
private:
public:
    glm::vec3 m_position;
    glm::vec3 m_direction = glm::vec3(p6::random::number(-1, 1), p6::random::number(-1, 1), p6::random::number(-1, 1));
    float     m_size      = 0.1f;
    Boid()
        : m_position({0, 0, -5}){};
    Boid(const glm::vec3& position, const float& size)
        : m_position(position), m_size(size){};

    void moove(p6::Context& ctx)
    {
        this->m_position += this->m_direction * ctx.delta_time();
    }

    void checkBorder(p6::Context& ctx, Environment environment)
    {
        if (this->m_position.x > environment.m_sizeX)
        {
            this->m_direction.x = -this->m_direction.x;
        }
        if (this->m_position.y > environment.m_sizeY)
        {
            this->m_direction.y = -this->m_direction.y;
        }
        if (this->m_position.x < -environment.m_sizeX)
        {
            this->m_direction.x = -this->m_direction.x;
        }
        if (this->m_position.y < -environment.m_sizeY)
        {
            this->m_direction.y = -this->m_direction.y;
        }
        if (this->m_position.z < -environment.m_sizeZ)
        {
            this->m_direction.z = -this->m_direction.z;
        }
        if (this->m_position.z > environment.m_sizeZ)
        {
            this->m_direction.z = -this->m_direction.z;
        }
    }
};