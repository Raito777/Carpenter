#pragma once

#include <stdlib.h>
#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <random>
#include <vector>
#include "Character.hpp"
#include "Environment.hpp"
#include "glm/fwd.hpp"
#include "glm/geometric.hpp"
#include "p6/p6.h"

class Boid {
private:
    glm::vec3 m_speed = glm::vec3(p6::random::number(0, 3), p6::random::number(0, 3), p6::random::number(0, 3));

    float m_speedFactor = 0.04;

    float     m_detectionRadius  = 0.5;
    float     m_avoidanceRadius  = 0.4;
    float     m_detectionFactor  = 1.0;
    float     m_avoidanceFactor  = 1.;
    float     m_separationFactor = 1.2;
    float     m_alignmentFactor  = 0.010;
    float     m_cohesionFactor   = 0.002;
    float     m_turnSpeedDefault = 0.04;
    glm::vec3 m_turnSpeed        = {m_turnSpeedDefault, m_turnSpeedDefault, m_turnSpeedDefault};
    float     m_turnFactor       = 0.04f;

    float m_cageMargin = 2.5;

    std::vector<Boid>
                      m_neighbors;
    std::vector<Boid> m_closeNeighbors;

    void checkBorder(const Environment& environment);

    // Check if there are neighbor boids around and store them in an array
    void updateNeighbors(const std::vector<Boid>& boids);

    void separation();

    // Get the general direction of the group of neighbors
    void alignement();

    void cohesion();

    void avoidBoundaries(const Environment& environment);

    void clearNeighborArrays();

public:
    glm::vec3 m_position;
    glm::vec3 m_direction = glm::vec3(p6::random::number(-1, 1), p6::random::number(-1, 1), p6::random::number(-1, 1));

    float m_size = 0.2f;

    Boid()
        : m_position({0, 0, -5}){};
    Boid(const glm::vec3& position, const float& size)
        : m_position(position), m_size(size){};

    void updatePosition(const Environment& environment, const std::vector<Boid>& boids);

    void updateSettings(const float& globalSpeedFactor, const float& globalDetectionFactor, const float& globalAvoidanceFactor, const float& globalSeparationFactor, const float& globalAlignmentFactor, const float& globalCohesionFactor);
};