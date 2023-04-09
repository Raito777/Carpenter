#pragma once

#include <stddef.h>
#include <vector>
#include "Boid.hpp"
#include "Environment.hpp"
#include "GLVao.hpp"
#include "GLVbo.hpp"
#include "OBJLoader.hpp"
#include "Scene.hpp"
#include "ShadowMapFBO.hpp"
#include "TrackballCamera.hpp"
#include "glimac/common.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/fwd.hpp"
#include "p6/p6.h"

class Renderer {
private:
    GLVbo        m_vboBoids;
    GLVbo        m_vboEnvironment;
    GLVao        m_vaoBoids;
    GLVao        m_vaoEnvironment;
    p6::Shader   m_shader             = p6::load_shader("shaders/3D.vs.glsl", "shaders/normals.fs.glsl");
    p6::Shader   m_shadowShader       = p6::load_shader("shaders/shadow.vs.glsl", "shaders/shadow.fs.glsl");
    float        m_valueOrtho         = 85.f;
    glm::mat4    m_shadowOrthoProjMat = glm::ortho(-m_valueOrtho, m_valueOrtho, -m_valueOrtho, m_valueOrtho, 0.1f, 75.0f);
    glm::mat4    m_lightView          = glm::lookAt(5.0f * glm::vec3(0, 0, 1), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
    glm::mat4    m_lightProjection    = m_shadowOrthoProjMat * m_lightView;
    ShadowMapFBO m_shadowMap;
    GLint        uMVPLight;

    void renderLights(p6::Context& ctx);
    void renderCamera(p6::Context& ctx);
    void renderBoids(p6::Context& ctx);
    void renderBoidsShadows(p6::Context& ctx);
    void renderEnvironment(p6::Context& ctx);

public:
    std::vector<Boid> m_boidsContainer;
    Environment       m_environment;
    Scene             m_scene;

    Renderer(){};
    Renderer(p6::Context& ctx, std::vector<Boid> boidsContainer, Environment environment);

    void render(p6::Context& ctx);
    void deleteBuffers();
};
