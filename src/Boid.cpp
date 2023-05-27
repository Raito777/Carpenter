#include "Boid.hpp"
#include <cmath>

// Check if there are neighbor boids around and store them in an array
void Boid::updateNeighbors(const std::vector<Boid>& boids)
{
    for (const auto& boid : boids)
    {
        if (&boid != this && glm::distance(m_position, boid.m_position) <= m_detectionRadius * 2)
        {
            m_neighbors.push_back(boid);
        }
    }
}

void Boid::separation()
{
    for (auto& m_neighbor : m_neighbors)
    {
        if (glm::distance(m_position, m_neighbor.m_position) < m_avoidanceRadius)
        {
            m_closeNeighbors.push_back(m_neighbor);
            m_speed += (m_position - m_neighbor.m_position) * m_separationFactor;
        }
    }
}

void Boid::alignement()
{
    glm::vec3 averageGroupSpeed{};
    for (auto& m_neighbor : m_neighbors)
    {
        averageGroupSpeed += m_neighbor.m_speed;
    }
    averageGroupSpeed.x = averageGroupSpeed.x / static_cast<float>(m_neighbors.size());
    averageGroupSpeed.y = averageGroupSpeed.y / static_cast<float>(m_neighbors.size());
    averageGroupSpeed.z = averageGroupSpeed.z / static_cast<float>(m_neighbors.size());

    m_speed += (averageGroupSpeed - m_speed) * m_alignmentFactor;
}

void Boid::cohesion()
{
    glm::vec3 averagePosition{};
    for (auto& m_neighbor : m_neighbors)
    {
        averagePosition += m_neighbor.m_position;
    }
    averagePosition.x = averagePosition.x / static_cast<float>(m_neighbors.size());
    averagePosition.y = averagePosition.y / static_cast<float>(m_neighbors.size());
    averagePosition.z = averagePosition.z / static_cast<float>(m_neighbors.size());

    m_speed += (averagePosition - m_speed) * m_cohesionFactor;
}

void Boid::avoidBoundaries(const Environment& environment)
{
    if (m_position.x < -environment.m_sizeX + m_cageMargin)
    {
        m_turnSpeed.x = (-(m_position.x - (-environment.m_sizeX + m_cageMargin))) * m_turnFactor;
        m_speed.x     = m_speed.x + m_turnSpeed.x;
    }
    else if (m_position.x > environment.m_sizeX - m_cageMargin)
    {
        m_turnSpeed.x = (m_position.x - (environment.m_sizeX - m_cageMargin)) * m_turnFactor;
        m_speed.x     = m_speed.x - m_turnSpeed.x;
    }
    else
    {
        m_turnSpeed.x = m_turnSpeedDefault;
    }
    if (m_position.y > environment.m_sizeY - m_cageMargin)
    {
        m_turnSpeed.y = (m_position.y - (environment.m_sizeY - m_cageMargin)) * m_turnFactor;
        m_speed.y     = m_speed.y - m_turnSpeed.y;
    }
    else if (m_position.y < -environment.m_sizeY + m_cageMargin)
    {
        m_turnSpeed.y = -((m_position.y - (-environment.m_sizeY + m_cageMargin))) * m_turnFactor;
        m_speed.y     = m_speed.y + m_turnSpeed.y;
    }
    else
    {
        m_turnSpeed.y = m_turnSpeedDefault;
    }

    if (m_position.z < -environment.m_sizeZ + m_cageMargin * 1.5)
    {
        m_turnSpeed.z = (-(m_position.z - (-environment.m_sizeZ + m_cageMargin * 1.5))) * m_turnFactor;
        m_speed.z     = m_speed.z + m_turnSpeed.z;
    }
    else if (m_position.z > environment.m_sizeZ - m_cageMargin * 1.5)
    {
        m_turnSpeed.x = (m_position.z - (environment.m_sizeZ - m_cageMargin * 1.5)) * m_turnFactor;
        m_speed.z     = m_speed.z - m_turnSpeed.z;
    }
    else
    {
        m_turnSpeed.z = m_turnSpeedDefault;
    }
}

// Move the Boid according to its parameters
void Boid::updatePosition(const Environment& environment, const std::vector<Boid>& boids)
{
    updateNeighbors(boids);
    if (!m_neighbors.empty())
    {
        separation();
        alignement();
        cohesion();
    }

    checkBorder(environment);
    m_direction = m_speed;
    m_position  = m_position + (m_speed * m_speedFactor);

    clearNeighborArrays();
}

void Boid::clearNeighborArrays()
{
    m_neighbors.clear();
    m_closeNeighbors.clear();
}

void Boid::checkBorder(const Environment& environment)
{
    // c'est pas mal avec ces valeurs
    if (this->m_position.x > environment.m_sizeX - m_cageMargin)
    {
        m_speed.x += -m_turnSpeedDefault;
    }
    if (this->m_position.y > environment.m_sizeY - m_cageMargin)
    {
        m_speed.y += -m_turnSpeedDefault;
    }
    if (this->m_position.x < -environment.m_sizeX + m_cageMargin)
    {
        m_speed.x += m_turnSpeedDefault;
    }
    if (this->m_position.y < -environment.m_sizeY + m_cageMargin)
    {
        m_speed.y += m_turnSpeedDefault;
    }
    if (this->m_position.z < -environment.m_sizeZ + m_cageMargin)
    {
        m_speed.z += m_turnSpeedDefault;
    }
    if (this->m_position.z > environment.m_sizeZ - m_cageMargin)
    {
        m_speed.z += -m_turnSpeedDefault;
    }
}

void Boid::updateSettings(const float& globalSpeedFactor, const float& globalDetectionFactor, const float& globalAvoidanceFactor, const float& globalSeparationFactor, const float& globalAlignmentFactor, const float& globalCohesionFactor)
{
    m_speedFactor      = globalSpeedFactor;
    m_detectionFactor  = globalDetectionFactor;
    m_avoidanceFactor  = globalAvoidanceFactor;
    m_separationFactor = globalSeparationFactor;
    m_alignmentFactor  = globalAlignmentFactor;
    m_cohesionFactor   = globalCohesionFactor;
}