#pragma once

#include <stdlib.h>
#include <iostream>
#include <random>
#include <vector>
#include "glm/ext/scalar_constants.hpp"
#include "glm/fwd.hpp"
#include "glm/geometric.hpp"
#include "p6/p6.h"

class Character {
private:
public:
    float m_sizeX = 0.2f;
    float m_sizeY = 0.2f;
    float m_sizeZ = 0.2f;

    float safetyDistance = 1.f;

    float m_speed = 10.f;

    float m_angleY = 0.f;

    glm::vec3 m_direction = glm::vec3(0, 0, 0);

    glm::vec3 m_position = glm::vec3(0, 0, 20);

    Character(){

    };
    Character(float sizeX, float sizeY, float sizeZ)
        : m_sizeX(sizeX), m_sizeY(sizeY), m_sizeZ(sizeZ){};

    void moveFront(p6::Context& ctx)
    {
        this->m_direction = glm::vec3(-sin(m_angleY), 0.f, -cos(m_angleY));

        this->m_position += glm::normalize(this->m_direction) * this->m_speed * ctx.delta_time();
    }
    void moveBack(p6::Context& ctx)
    {
        this->m_direction = glm::vec3(-sin(m_angleY), 0.f, -cos(m_angleY));

        this->m_position -= glm::normalize(this->m_direction) * this->m_speed * ctx.delta_time();
    }
    void moveLeft(p6::Context& ctx)
    {
        this->m_direction = glm::vec3(-sin(m_angleY), 0.f, -cos(m_angleY));

        glm::vec3 perp = glm::normalize(glm::cross(m_direction, glm::vec3(-1, 0, 0)));

        glm::vec3 left = glm::normalize(glm::cross(perp, m_direction));

        if (m_angleY > glm::pi<float>() / 2 || m_angleY < -(glm::pi<float>() / 2))
        {
            this->m_position -= left * this->m_speed * ctx.delta_time();
        }
        else
        {
            this->m_position += left * this->m_speed * ctx.delta_time();
        }
    }
    void moveRight(p6::Context& ctx)
    {
        this->m_direction = glm::vec3(-sin(m_angleY), 0.f, -cos(m_angleY));

        glm::vec3 perp = glm::normalize(glm::cross(m_direction, glm::vec3(1, 0, 0)));

        glm::vec3 left = glm::normalize(glm::cross(perp, m_direction));

        if (m_angleY > glm::pi<float>() / 2 || m_angleY < -(glm::pi<float>() / 2))
        {
            this->m_position -= left * this->m_speed * ctx.delta_time();
        }
        else
        {
            this->m_position += left * this->m_speed * ctx.delta_time();
        }
    }
};