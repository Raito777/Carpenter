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

    void initLightGlints(const p6::Shader& shader)
    {
        this->uLightPos_vs    = glGetUniformLocation(shader.id(), "uLightPos_vs");
        this->uLightIntensity = glGetUniformLocation(shader.id(), "uLightIntensity");
        this->uAmbient        = glGetUniformLocation(shader.id(), "uKa");
    }
};

struct lightTexture {
    GLint uKd;
    GLint uKs;
    GLint uShininess;

    std::vector<glm::vec3> _uKd;
    std::vector<glm::vec3> _uKs;
    std::vector<float>     _uShininess;

    void initLightTexture(const p6::Shader& shader)
    {
        this->uKd        = glGetUniformLocation(shader.id(), "uKd");
        this->uKs        = glGetUniformLocation(shader.id(), "uKs");
        this->uShininess = glGetUniformLocation(shader.id(), "uShininess");
    }
};

struct modelTransformations {
    glm::mat4 ProjMatrix;
    glm::mat4 MVMatrix;
    glm::mat4 NormalMatrix;

    GLint uMVPMatrix;
    GLint uMVMatrix;
    GLint uNormalMatrix;

    void initModelTransformations(p6::Context& ctx, const p6::Shader& shader)
    {
        this->uMVPMatrix    = glGetUniformLocation(shader.id(), "uMVPMatrix");
        this->uMVMatrix     = glGetUniformLocation(shader.id(), "uMVMatrix");
        this->uNormalMatrix = glGetUniformLocation(shader.id(), "uNormalMatrix");
        this->ProjMatrix    = glm::perspective(glm::radians(70.f), (GLfloat)ctx.aspect_ratio(), 0.1f, 100.f);
        this->MVMatrix      = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
        this->NormalMatrix  = glm::transpose(glm::inverse(this->MVMatrix));
    }
};

class Scene {
private:
public:
    std::vector<glimac::ShapeVertex> m_boidModel;
    modelTransformations             m_boidsTransformations;
    lightTexture                     m_boidLightTexture;

    std::vector<glimac::ShapeVertex> m_environmentModel;
    modelTransformations             m_environmentTransformations;

    std::vector<sceneLightSetup> m_lights;
    TrackballCamera              m_camera;
    Scene();
    Scene(p6::Context& ctx, p6::Shader shader)
    {
        this->m_boidModel        = loadOBJ("./assets/models/bat2.obj");
        this->m_environmentModel = loadOBJ("./assets/models/CaveRock_L_Obj.obj");

        sceneLightSetup aLight;
        aLight._lightPos        = glm::vec3(0, 0, -3);
        aLight._uLightIntensity = glm::vec3(3.f, 3.f, 3.f);
        aLight._uAmbient        = glm::vec3(0.1f, 0.1f, 0.1f);

        this->m_lights.push_back(aLight);

        for (size_t i = 0; i < this->m_lights.size(); i++)
        {
            this->m_lights[i].initLightGlints(shader);
        }

        m_boidsTransformations.initModelTransformations(ctx, shader);
        m_environmentTransformations.initModelTransformations(ctx, shader);
    };
};