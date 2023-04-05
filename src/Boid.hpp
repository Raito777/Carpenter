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
    glm::vec3 m_direction = glm::vec3(p6::random::number(-0.01, 0.01), p6::random::number(-0.01, 0.01), p6::random::number(-0.01, 0.01));
    float     m_size      = 1.f;
    Boid()
        : m_position({0, 0, -5}){};
    Boid(const glm::vec3& position, const float& size)
        : m_position(position), m_size(size){};

    void moove()
    {
        this->m_position += this->m_direction;
    }

    void checkBorder(p6::Context& ctx)
    {
        if (this->m_position.x > ctx.aspect_ratio())
        {
            this->m_direction.x = -this->m_direction.x;
        }
        if (this->m_position.y > 1)
        {
            this->m_direction.y = -this->m_direction.y;
        }
        if (this->m_position.x < -ctx.aspect_ratio())
        {
            this->m_direction.x = -this->m_direction.x;
        }
        if (this->m_position.y < -1)
        {
            this->m_direction.y = -this->m_direction.y;
        }
        if (this->m_position.z < -5)
        {
            this->m_direction.z = -this->m_direction.z;
        }
        if (this->m_position.z > 0)
        {
            this->m_direction.z = -this->m_direction.z;
        }
    }
};