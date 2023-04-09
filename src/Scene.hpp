#pragma once

#include <stddef.h>
#include <vector>
#include "OBJLoader.hpp"
#include "TrackballCamera.hpp"
#include "glimac/common.hpp"
#include "glimac/cone_vertices.hpp"
#include "glm/fwd.hpp"
#include "p6/p6.h"

struct sceneLightSetup {
    GLint uLightPos_vs;
    GLint uLightDir_vs;

    GLint uLightIntensity;
    GLint uAmbient;

    glm::vec3 _uLightIntensity;
    glm::vec3 _lightPos;
    glm::vec3 _lightDir;

    glm::vec3 _uAmbient;

    void initPointLightGlints(const unsigned int shaderId)
    {
        this->uLightPos_vs    = glGetUniformLocation(shaderId, "uLightPos_vs_Light1");
        this->uLightIntensity = glGetUniformLocation(shaderId, "uLightIntensity_Light1");
        this->uAmbient        = glGetUniformLocation(shaderId, "uKa_Light1");
    }

    void initDirLightGlints(const unsigned int shaderId)
    {
        this->uLightDir_vs    = glGetUniformLocation(shaderId, "uLightDir_vs_Light2");
        this->uLightIntensity = glGetUniformLocation(shaderId, "uLightIntensity_Light2");
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
    int shadow_width  = 4096;
    int shadow_height = 4096;

    std::vector<glimac::ShapeVertex> m_boidModel;
    modelTransformations             m_boidsTransformations;
    lightTexture                     m_boidLightTexture;

    std::vector<glimac::ShapeVertex> m_environmentModel;
    modelTransformations             m_environmentTransformations;
    lightTexture                     m_environmentLightTexture;

    sceneLightSetup m_pointLight;
    sceneLightSetup m_dirLight;

    TrackballCamera m_camera;

    Scene(){};

    Scene(p6::Context& ctx, const unsigned int shaderId)
    {
        this->m_boidModel = loadOBJ("./assets/models/bat2.obj");
        // this->m_boidModel        = glimac::cone_vertices(1.f, 0.5f, 16, 32);
        this->m_environmentModel = loadOBJ("./assets/models/close-cube.obj");

        m_pointLight._lightPos        = glm::vec3(0, 0, -0.5);
        m_pointLight._uLightIntensity = glm::vec3(5.f, 5.f, 5.f);
        m_pointLight._uAmbient        = glm::vec3(0.0f, 0.0f, 0.0f);

        m_dirLight._lightDir        = glm::vec3(0.0, 1, 0.0);
        m_dirLight._uLightIntensity = glm::vec3(0.2f, 0.2f, 0.2f);

        m_pointLight.initPointLightGlints(shaderId);
        m_dirLight.initDirLightGlints(shaderId);

        m_boidsTransformations.initModelTransformations(ctx, shaderId);
        m_environmentTransformations.initModelTransformations(ctx, shaderId);

        m_boidLightTexture.initLightTexture(shaderId);
        m_environmentLightTexture.initLightTexture(shaderId);

        this->m_boidLightTexture._uKd.push_back(glm::vec3(1.f, 0.1f, 0.1f));
        this->m_boidLightTexture._uKs.push_back(glm::vec3(1.f, 0.1f, 0.1f));
        this->m_boidLightTexture._uShininess.push_back(1.f);

        this->m_environmentLightTexture._uKd.push_back(glm::vec3(0.1f, 0.1f, 1.f));
        this->m_environmentLightTexture._uKs.push_back(glm::vec3(0.1f, 0.1f, 1.f));
        this->m_environmentLightTexture._uShininess.push_back(1.f);
    };

    void updateGlints(p6::Context& ctx, const unsigned int shaderId)
    {
        m_pointLight.initPointLightGlints(shaderId);
        m_dirLight.initDirLightGlints(shaderId);

        m_boidsTransformations.initModelTransformations(ctx, shaderId);
        m_environmentTransformations.initModelTransformations(ctx, shaderId);

        m_boidLightTexture.initLightTexture(shaderId);
        m_environmentLightTexture.initLightTexture(shaderId);
    }
};