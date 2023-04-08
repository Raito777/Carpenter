#pragma once

#include <stddef.h>
#include <vector>
#include "Boid.hpp"
#include "GLVbo.hpp"
#include "OBJLoader.hpp"
#include "Scene.hpp"
#include "TrackballCamera.hpp"
#include "glimac/common.hpp"
#include "glm/fwd.hpp"
#include "p6/p6.h"

class Renderer {
private:
public:
    std::vector<Boid>  m_boidsContainer;
    p6::Shader         m_shader = p6::load_shader("shaders/3D.vs.glsl", "shaders/normals.fs.glsl");
    Scene              m_scene;
    std::vector<GLVbo> m_vbo;

    Renderer(p6::Context& ctx)
    {
        this->m_scene = Scene(ctx, this->m_shader.id());

        this->m_vbo.push_back(GLVbo());
        this->m_vbo.push_back(GLVbo());

        this->m_vbo[0].bind();
        glBufferData(GL_ARRAY_BUFFER, this->m_scene.m_boidModel.size() * sizeof(glimac::ShapeVertex), this->m_scene.m_boidModel.data(), GL_STATIC_DRAW);
        this->m_vbo[0].unBind();

        this->m_vbo[1].bind();
        glBufferData(GL_ARRAY_BUFFER, this->m_scene.m_boidModel.size() * sizeof(glimac::ShapeVertex), this->m_scene.m_boidModel.data(), GL_STATIC_DRAW);
        this->m_vbo[0].unBind();
    }

    void render()
    {
    }
};