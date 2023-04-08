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

    void initLightGlints(const unsigned int shaderId)
    {
        this->uLightPos_vs    = glGetUniformLocation(shaderId, "uLightPos_vs");
        this->uLightIntensity = glGetUniformLocation(shaderId, "uLightIntensity");
        this->uAmbient        = glGetUniformLocation(shaderId, "uKa");
    }
};

struct lightTexture {
    GLint uKd;
    GLint uKs;
    GLint uShininess;

    std::vector<glm::vec3> _uKd;
    std::vector<glm::vec3> _uKs;
    std::vector<float>     _uShininess;

    void initLightTexture(const unsigned int shaderId)
    {
        this->uKd        = glGetUniformLocation(shaderId, "uKd");
        this->uKs        = glGetUniformLocation(shaderId, "uKs");
        this->uShininess = glGetUniformLocation(shaderId, "uShininess");
    }
};

struct modelTransformations {
    glm::mat4 ProjMatrix;
    glm::mat4 MVMatrix;
    glm::mat4 NormalMatrix;

    GLint uMVPMatrix;
    GLint uMVMatrix;
    GLint uNormalMatrix;

    void initModelTransformations(p6::Context& ctx, const unsigned int shaderId)
    {
        this->uMVPMatrix    = glGetUniformLocation(shaderId, "uMVPMatrix");
        this->uMVMatrix     = glGetUniformLocation(shaderId, "uMVMatrix");
        this->uNormalMatrix = glGetUniformLocation(shaderId, "uNormalMatrix");
        this->ProjMatrix    = glm::perspective(glm::radians(70.f), (GLfloat)ctx.aspect_ratio(), 0.1f, 100.f);
        this->MVMatrix      = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, -2));
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
    lightTexture                     m_environmentLightTexture;

    std::vector<sceneLightSetup> m_lights;
    TrackballCamera              m_camera;

    Scene(){};

    Scene(p6::Context& ctx, const unsigned int shaderId)
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
            this->m_lights[i].initLightGlints(shaderId);
        }

        m_boidsTransformations.initModelTransformations(ctx, shaderId);
        m_environmentTransformations.initModelTransformations(ctx, shaderId);

        m_boidLightTexture.initLightTexture(shaderId);
        m_environmentLightTexture.initLightTexture(shaderId);

        this->m_boidLightTexture._uKd.push_back(glm::vec3(1.f, 0.1f, 0.1f));
        this->m_boidLightTexture._uKs.push_back(glm::vec3(1.f, 0.1f, 0.1f));
        this->m_boidLightTexture._uShininess.push_back(0.4f);

        this->m_environmentLightTexture._uKd.push_back(glm::vec3(0.1f, 0.1f, 1.f));
        this->m_environmentLightTexture._uKs.push_back(glm::vec3(0.1f, 0.1f, 1.f));
        this->m_environmentLightTexture._uShininess.push_back(1.f);
    };
};