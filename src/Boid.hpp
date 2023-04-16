#pragma once

#include <stdlib.h>
#include <random>
#include <vector>
#include "Character.hpp"
#include "Environment.hpp"
#include "glm/fwd.hpp"
#include "glm/geometric.hpp"
#include "p6/p6.h"

class Boid {
private:
public:
    glm::vec3 m_position;
    glm::vec3 m_direction = glm::vec3(p6::random::number(-1, 1), p6::random::number(-1, 1), p6::random::number(-1, 1));
    float     m_speed     = 1.5f;

    float m_size = 0.1f;
    Boid()
        : m_position({0, 0, -5}){};
    Boid(const glm::vec3& position, const float& size)
        : m_position(position), m_size(size){};

    void moove(p6::Context& ctx)
    {
        this->m_position += this->m_direction * this->m_speed * ctx.delta_time();
    }

    void checkBorder(p6::Context& ctx, Environment environment)
    {
        // c'est pas mal avec ces valeurs
        if (this->m_position.x > environment.m_sizeX - 2)
        {
            this->m_direction.x += -0.4f;
        }
        if (this->m_position.y > environment.m_sizeY - 2)
        {
            this->m_direction.y += -0.4f;
        }
        if (this->m_position.x < -environment.m_sizeX + 2)
        {
            this->m_direction.x += 0.4f;
        }
        if (this->m_position.y < -environment.m_sizeY + 2)
        {
            this->m_direction.y += 0.4f;
        }
        if (this->m_position.z < -environment.m_sizeZ + 2)
        {
            this->m_direction.z += 0.4f;
        }
        if (this->m_position.z > environment.m_sizeZ - 2)
        {
            this->m_direction.z += -0.4f;
        }
    }

    void avoidCharacter(p6::Context& ctx, Character character)
    {
        glm::vec3 totalForce(0.0f, 0.0f, 0.0f);

        float distance = glm::distance(this->m_position, character.m_position);
        totalForce     = (this->m_position - character.m_position) / (distance * 50.f);

        // Calculer la direction à partir du vecteur diff
        this->m_direction += totalForce;
    }
};