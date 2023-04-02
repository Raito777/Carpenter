#pragma once

#include <random>
#include <vector>
#include "glm/ext.hpp"
#include "p6/p6.h"

class GLlight {
private:
    GLint m_uAmbient;
    GLint m_uKd;
    GLint m_uKs;
    GLint m_uShininess;
    GLint m_uLightPos_vs;
    GLint m_uLightIntensity;

    std::vector<glm::vec3> _uKd;
    std::vector<glm::vec3> _uKs;
    std::vector<float>     _uShininess;
    std::vector<glm::vec3> _uLightIntensity;

public:
    glm::vec3 m_lightPos;

    GLlight(p6::Shader shader)
    {
        this->m_uAmbient        = glGetUniformLocation(shader.id(), "uKa");
        this->m_uKd             = glGetUniformLocation(shader.id(), "uKd");
        this->m_uKs             = glGetUniformLocation(shader.id(), "uKs");
        this->m_uShininess      = glGetUniformLocation(shader.id(), "uShininess");
        this->m_uLightPos_vs    = glGetUniformLocation(shader.id(), "uLightPos_vs");
        this->m_uLightIntensity = glGetUniformLocation(shader.id(), "uLightIntensity");
    };
    void setupLight()
    {
        this->_uKd.push_back(glm::vec3(glm::linearRand(0.f, 1.0f), glm::linearRand(0.f, 1.0f), glm::linearRand(0.f, 1.0f)));
        this->_uKs.push_back(glm::vec3(glm::linearRand(0.f, 1.0f), glm::linearRand(0.f, 1.0f), glm::linearRand(0.f, 1.0f)));
        this->_uLightIntensity.push_back(glm::vec3(glm::linearRand(0.f, 1.0f), glm::linearRand(0.f, 1.0f), glm::linearRand(0.f, 1.0f)));
        this->_uShininess.push_back(glm::linearRand(0.f, 1.0f));
    }
    void updateLight()
    {
        glUniform3fv(this->m_lightSetup.uKd, 1, glm::value_ptr(this->_uKd[i]));
        glUniform3fv(this->m_lightSetup.uKs, 1, glm::value_ptr(this->_uKs[i]));
        glUniform1f(this->m_lightSetup.uShininess, this->_uShininess[i]);
    }
};