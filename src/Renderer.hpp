#pragma once

#include <stddef.h>
#include <vector>
#include "Boid.hpp"
#include "OBJLoader.hpp"
#include "Scene.hpp"
#include "TrackballCamera.hpp"
#include "glimac/common.hpp"
#include "glm/fwd.hpp"
#include "p6/p6.h"

class Renderer {
private:
public:
    std::vector<Boid> m_boidsContainer;
    p6::Shader        m_shader = p6::load_shader("shaders/3D.vs.glsl", "shaders/normals.fs.glsl");
    Scene             m_scene;

    Renderer(p6::Context& ctx)
    {
        this->m_scene = Scene(ctx, this->m_shader.id());
    }

    void render();
};