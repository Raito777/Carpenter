#pragma once

#include <vector>
#include "glm/ext.hpp"

class TrackballCamera {
public:
    TrackballCamera()
        : m_fDistance(-3), m_fAngleX(0.f), m_fAngleY(0.f){};

    void moveFront(float delta)
    {
        m_fDistance += delta;
    }
    void rotateLeft(float degrees)
    {
        m_fAngleX += degrees;
    }
    void rotateUp(float degrees)
    {
        m_fAngleY += degrees;
    }

    glm::mat4 getViewMatrix() const
    {
        glm::mat4 MVMatrix;
        MVMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, this->m_fDistance));
        MVMatrix = glm::rotate(MVMatrix, glm::radians(-this->m_fAngleX), glm::vec3(1, 0, 0));
        MVMatrix = glm::rotate(MVMatrix, glm::radians(this->m_fAngleY), glm::vec3(0, 1, 0));

        return MVMatrix;
    }

    float getAngleY() const
    {
        return m_fAngleY;
    }

    float getAngleX() const
    {
        return m_fAngleY;
    }

private:
    float m_fDistance, m_fAngleX, m_fAngleY;
};