#include "GLContext.hpp"
#include <vcruntime.h>
#include <glm/gtx/vector_angle.hpp>
#include <iostream>
#include <string_view>
#include <vector>
#include "Boid.hpp"
#include "Environment.hpp"
#include "OBJLoader.hpp"
#include "Renderer.hpp"
#include "Scene.hpp"
#include "glimac/common.hpp"
#include "glimac/cone_vertices.hpp"
#include "glimac/sphere_vertices.hpp"
#include "glm/ext.hpp"
#include "p6/p6.h"

GLContext::GLContext(p6::Context& ctx, std::vector<Boid>& boidsContainer, Environment environment)
{
    glEnable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);

    this->m_renderer = Renderer(ctx, boidsContainer, environment);

    ctx.mouse_scrolled = [&](p6::MouseScroll data) {
        this->m_renderer.m_scene.m_camera.moveFront(data.dy);
    };
}

void GLContext::draw(p6::Context& ctx)
{
    this->m_renderer.render(ctx);
}

void GLContext::deleteBuffers()
{
    this->m_renderer.deleteBuffers();
}