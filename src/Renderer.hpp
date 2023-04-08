#pragma once

#include <stddef.h>
#include <vector>
#include "Boid.hpp"
#include "GLVao.hpp"
#include "GLVbo.hpp"
#include "OBJLoader.hpp"
#include "Scene.hpp"
#include "TrackballCamera.hpp"
#include "glimac/common.hpp"
#include "glm/fwd.hpp"
#include "p6/p6.h"

class Renderer {
private:
    GLVbo      m_vboBoids;
    GLVbo      m_vboEnvironment;
    GLVao      m_vaoBoids;
    GLVao      m_vaoEnvironment;
    p6::Shader m_shader = p6::load_shader("shaders/3D.vs.glsl", "shaders/normals.fs.glsl");

    void renderLights();
    void renderCamera(p6::Context& ctx);
    void renderBoids(p6::Context& ctx);
    void renderEnvironment(p6::Context& ctx);

public:
    std::vector<Boid> m_boidsContainer;
    Scene             m_scene;

    Renderer(){};
    Renderer(p6::Context& ctx, std::vector<Boid> boidsContainer);

    void render(p6::Context& ctx);
    void deleteBuffers();
};
