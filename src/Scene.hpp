#pragma once

#include <stddef.h>
#include <vector>
#include "Boid.hpp"
#include "Character.hpp"
#include "Environment.hpp"
#include "OBJLoader.hpp"
#include "ShadowMapFBO.hpp"
#include "TrackballCamera.hpp"
#include "glimac/common.hpp"
#include "glimac/cone_vertices.hpp"
#include "glm/fwd.hpp"
#include "p6/p6.h"

struct sceneLightSetup {
    GLint uLightPos_vs;
    GLint uLightPos;
    GLint uLightDir_vs;
    GLint uLightIntensity;
    GLint uAmbient;

    glm::vec3 _uLightIntensity;
    glm::vec3 lightColor;
    glm::vec3 initialLightIntensity;
    glm::vec3 _lightPos;
    glm::vec3 initialLightPos;
    glm::vec3 _lightDir;
    glm::vec3 _uAmbient;

    void initPointLightGlints(const unsigned int shaderId)
    {
        this->uLightPos_vs    = glGetUniformLocation(shaderId, "uLightPos_vs_Light1");
        this->uLightIntensity = glGetUniformLocation(shaderId, "uLightIntensity_Light1");
        this->uAmbient        = glGetUniformLocation(shaderId, "uKa_Light1");
        this->uLightPos       = glGetUniformLocation(shaderId, "uLightPos_Light1");
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
    GLint uTexture;
    GLint ufar_plane;
    GLint uLightPos;

    std::vector<glm::vec3> _uKd;
    std::vector<glm::vec3> _uKs;
    std::vector<float>     _uShininess;

    void initLightTexture(const unsigned int shaderId)
    {
        this->uKd        = glGetUniformLocation(shaderId, "uKd");
        this->uKs        = glGetUniformLocation(shaderId, "uKs");
        this->uShininess = glGetUniformLocation(shaderId, "uShininess");
        this->uTexture   = glGetUniformLocation(shaderId, "uTexture");
        this->ufar_plane = glGetUniformLocation(shaderId, "ufar_plane");
        this->uLightPos  = glGetUniformLocation(shaderId, "uLightPos");
    }
};

struct objectTexture {
    img::Image imgTexture = p6::load_image_buffer("./assets/textures/defaultTexture.png");
    GLuint     texture;
    GLint      uTexture;

    objectTexture(std::filesystem::path file_path)
    {
        this->imgTexture = p6::load_image_buffer(file_path);
        std::cout << file_path << "\n";
        genTexture();
    }

    void genTexture()
    {
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imgTexture.width(), imgTexture.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, imgTexture.data());
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        glBindTexture(GL_TEXTURE_2D, 0);
    }
    void setTexture(std::filesystem::path file_path)
    {
        this->imgTexture = p6::load_image_buffer(file_path);
        genTexture();
    }
    void bindTexture(GLenum textureUnit)
    {
        glActiveTexture(textureUnit);
        glBindTexture(GL_TEXTURE_2D, this->texture);
    }

    void unbindTexture()
    {
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    void updateGlint(int shaderId)
    {
        uTexture = glGetUniformLocation(shaderId, "uTextureImg");
    }
};

struct modelTransformations {
    glm::mat4 ProjMatrix;
    glm::mat4 MVMatrix;
    glm::mat4 NormalMatrix;
    glm::mat4 MMatrix;

    GLint uMVPMatrix;
    GLint uMVMatrix;
    GLint uNormalMatrix;
    GLint uMMatrix;

    void initModelTransformations(p6::Context& ctx, const unsigned int shaderId)
    {
        this->uMVPMatrix    = glGetUniformLocation(shaderId, "uMVPMatrix");
        this->uMVMatrix     = glGetUniformLocation(shaderId, "uMVMatrix");
        this->uNormalMatrix = glGetUniformLocation(shaderId, "uNormalMatrix");
        this->uMMatrix      = glGetUniformLocation(shaderId, "uMMatrix");

        this->ProjMatrix   = glm::perspective(glm::radians(70.f), (GLfloat)ctx.aspect_ratio(), 0.1f, 100.f);
        this->MVMatrix     = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, -2));
        this->NormalMatrix = glm::transpose(glm::inverse(this->MVMatrix));
    }
};

class Scene {
private:
public:
    int shadow_width  = 4096;
    int shadow_height = 4096;

    std::vector<Boid> m_boids;

    std::vector<glimac::ShapeVertex> m_boidModel;
    std::vector<glimac::ShapeVertex> m_boidModel1;
    std::vector<glimac::ShapeVertex> m_boidModel2;
    std::vector<glimac::ShapeVertex> m_boidModel3;

    modelTransformations m_boidsTransformations;
    lightTexture         m_boidLightTexture;
    objectTexture        m_boidTextures = objectTexture("./assets/textures/batDetailed.jpg");
    // objectTexture        m_boidTextures1 = objectTexture("./assets/textures/bat.png");
    // objectTexture        m_boidTextures2 = objectTexture("./assets/textures/defaultTexture.png");
    // objectTexture        m_boidTextures3 = objectTexture("./assets/textures/batDetailed.jpg");
    std::vector<glimac::ShapeVertex> m_cageModel;
    std::vector<glimac::ShapeVertex> m_environmentModel;
    std::vector<glimac::ShapeVertex> m_environmentModel1;
    std::vector<glimac::ShapeVertex> m_environmentModel2;

    modelTransformations m_environmentTransformations;
    lightTexture         m_environmentLightTexture;
    objectTexture        m_environmentTextures = objectTexture("./assets/textures/defaultTexture.png");

    std::vector<glimac::ShapeVertex> m_characterModel;
    modelTransformations             m_characterTransformations;
    lightTexture                     m_characterLightTexture;
    objectTexture                    m_characterTexture = objectTexture("./assets/textures/robot.jpg");

    Environment m_environment;
    Environment m_cage;

    Character m_character;

    sceneLightSetup m_pointLight;
    sceneLightSetup m_dirLight;

    TrackballCamera m_camera;

    Scene(){};

    Scene(p6::Context& ctx, const unsigned int shaderId)
    {
        m_boidTextures.updateGlint(shaderId);
        m_environmentTextures.updateGlint(shaderId);

        this->m_boidModel1 = loadOBJ("./assets/models/BatLow.obj");
        this->m_boidModel2 = loadOBJ("./assets/models/BatMid.obj");
        this->m_boidModel3 = loadOBJ("./assets/models/BatDetailed.obj");

        this->m_boidModel = this->m_boidModel3;

        // this->m_boidModel        = glimac::cone_vertices(1.f, 0.5f, 16, 32);
        this->m_environmentModel1 = loadOBJ("./assets/models/cave_v1.obj");
        this->m_environmentModel2 = loadOBJ("./assets/models/modular-cube.obj");

        this->m_environmentModel = this->m_environmentModel1;

        this->m_cageModel = this->m_environmentModel2;

        this->m_characterModel = loadOBJ("./assets/models/robot.obj");

        m_pointLight._lightPos       = glm::vec3(-2, 0, -0.5);
        m_pointLight.initialLightPos = m_pointLight._lightPos;

        m_pointLight.lightColor            = glm::vec3(0.7, 0.5, 0.5);
        m_pointLight._uLightIntensity      = m_pointLight.lightColor * 70.f;
        m_pointLight.initialLightIntensity = m_pointLight.lightColor * 70.f;

        m_pointLight._uAmbient = glm::vec3(0.08f, 0.08f, 0.08f);

        m_dirLight._lightDir        = glm::vec3(0.0, 1, 0.0);
        m_dirLight._uLightIntensity = glm::vec3(0.f, 0.f, 0.f);

        m_pointLight.initPointLightGlints(shaderId);
        m_dirLight.initDirLightGlints(shaderId);

        m_boidsTransformations.initModelTransformations(ctx, shaderId);
        m_environmentTransformations.initModelTransformations(ctx, shaderId);
        m_characterTransformations.initModelTransformations(ctx, shaderId);

        m_boidLightTexture.initLightTexture(shaderId);
        m_environmentLightTexture.initLightTexture(shaderId);
        m_characterLightTexture.initLightTexture(shaderId);

        // couleur reflétée
        this->m_boidLightTexture._uKd.push_back(glm::vec3(0.2f, 0.2f, 0.2f));
        // couleur spéculaire reflétée dans une direction spécifique en fonction de l'angle de la
        this->m_boidLightTexture._uKs.push_back(glm::vec3(0.2f, 0.2f, 0.2f));
        // rugositée
        this->m_boidLightTexture._uShininess.push_back(1.f);

        this->m_environmentLightTexture._uKd.push_back(glm::vec3(9.f, 9.f, 9.f));
        this->m_environmentLightTexture._uKs.push_back(glm::vec3(0.01f, 0.01f, 0.01f));
        this->m_environmentLightTexture._uShininess.push_back(0.05f);

        this->m_characterLightTexture._uKd.push_back(glm::vec3(0.1f, 0.1f, 0.1f));
        this->m_characterLightTexture._uKs.push_back(glm::vec3(0.1f, 0.1f, 0.1f));
        this->m_characterLightTexture._uShininess.push_back(0.5f);

        this->m_environment = Environment(1, 1, 1);
        this->m_cage        = Environment(5, 5, 20);

        for (size_t i = 0; i < 50; i++)
        {
            glm::vec3 position{p6::random::number(-m_environment.m_sizeX, m_environment.m_sizeX), p6::random::number(-m_environment.m_sizeY, m_environment.m_sizeY), p6::random::number(-m_environment.m_sizeZ, m_environment.m_sizeZ)};
            float     size = 0.1f;
            // float size = 0.1f;
            Boid boid(position, size);
            m_boids.push_back(boid);
        }
    };

    void updateGlints(p6::Context& ctx, const unsigned int shaderId)
    {
        m_pointLight.initPointLightGlints(shaderId);
        m_dirLight.initDirLightGlints(shaderId);

        m_boidsTransformations.initModelTransformations(ctx, shaderId);
        m_environmentTransformations.initModelTransformations(ctx, shaderId);
        m_characterTransformations.initModelTransformations(ctx, shaderId);

        m_boidLightTexture.initLightTexture(shaderId);
        m_environmentLightTexture.initLightTexture(shaderId);
        m_characterLightTexture.initLightTexture(shaderId);

        m_boidTextures.updateGlint(shaderId);
        m_environmentTextures.updateGlint(shaderId);
    }

    void setLight1Intensity(float lightAdjustment)
    {
        this->m_pointLight._uLightIntensity = this->m_pointLight.initialLightIntensity * lightAdjustment;
    }

    void setBoidModel(int boidNumModel)
    {
        if (boidNumModel == 1)
        {
            this->m_boidModel = this->m_boidModel1;
            this->m_boidTextures.setTexture("./assets/textures/bat.png");
        }

        if (boidNumModel == 2)
        {
            this->m_boidModel = this->m_boidModel2;
            this->m_boidTextures.setTexture("./assets/textures/defaultTexture.png");
        }
        if (boidNumModel == 3)
        {
            this->m_boidModel = this->m_boidModel3;
            this->m_boidTextures.setTexture("./assets/textures/batDetailed.jpg");
        }
    }

    void setEnvironmentModel(int environmentModel)
    {
        if (environmentModel == 1)
        {
            this->m_environmentModel = this->m_environmentModel1;
        }

        if (environmentModel == 2)
        {
            this->m_environmentModel = this->m_environmentModel2;
        }
    }

    void setLightColor(float* lightColor, float lightAdjustment)
    {
        glm::vec3 vecLightColor(lightColor[0], lightColor[1], lightColor[2]);
        m_pointLight._uLightIntensity = vecLightColor * lightAdjustment;
    }
};