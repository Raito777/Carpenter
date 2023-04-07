#pragma once

#include <stddef.h>
#include <vector>
#include "OBJLoader.hpp"
#include "TrackballCamera.hpp"
#include "glimac/common.hpp"
#include "glm/fwd.hpp"
#include "p6/p6.h"

struct sceneLightSetup {
    GLint uLightPos_vs;
    GLint uLightIntensity;
    GLint uAmbient;

    glm::vec3 _uLightIntensity;
    glm::vec3 _lightPos;
    glm::vec3 _uAmbient;
};

struct modelTransformations {
    glm::mat4 ProjMatrix;
    glm::mat4 MVMatrix;
    glm::mat4 NormalMatrix;
};

class Scene {
private:
public:
    std::vector<glimac::ShapeVertex> m_BoidModel;
    modelTransformations             m_BoidsTransformations;

    std::vector<glimac::ShapeVertex> m_EnvironmentModel;
    modelTransformations             m_EnvironmentTransformations;

    std::vector<sceneLightSetup> m_lights;
    TrackballCamera              m_camera;

    Scene()
    {
        this->m_BoidModel        = loadOBJ("./assets/models/bat2.obj");
        this->m_EnvironmentModel = loadOBJ("./assets/models/CaveRock_L_Obj.obj");

        sceneLightSetup aLight;
        aLight._lightPos        = glm::vec3(0, 0, -3);
        aLight._uLightIntensity = glm::vec3(3.f, 3.f, 3.f);
        aLight._uAmbient        = glm::vec3(0.1f, 0.1f, 0.1f);

        this->m_lights.push_back(aLight);
    };

    void setLightGlints(const p6::Shader& shader)
    {
        for (size_t i = 0; i < this->m_lights.size(); i++)
        {
            this->m_lights[i].uLightPos_vs    = glGetUniformLocation(shader.id(), "uLightPos_vs");
            this->m_lights[i].uLightIntensity = glGetUniformLocation(shader.id(), "uLightIntensity");
            this->m_lights[i].uAmbient        = glGetUniformLocation(shader.id(), "uKa");
        }
    }

    void setModelTransformations()
    {
    }
};