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

GLContext::GLContext(p6::Context& ctx, std::vector<Boid>& boidsContainer)
{
    glEnable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);

    m_shadowMap.Init(2048);
    std::cout << "1 : " << &m_shadowMap << "\n";

    this->m_renderer = Renderer(ctx, boidsContainer);

    ctx.mouse_scrolled = [&](p6::MouseScroll data) {
        this->m_renderer.m_scene.m_camera.moveFront(data.dy);
    };

    ctx.key_pressed = [&](const p6::Key& key) {
        if (key.physical == GLFW_KEY_W)
        {
            this->Z = true;
        }
        if (key.physical == GLFW_KEY_A)
        {
            this->Q = true;
        }
        if (key.physical == GLFW_KEY_S)
        {
            this->S = true;
        }
        if (key.physical == GLFW_KEY_D)
        {
            this->D = true;
        }
    };

    ctx.key_released = [&](const p6::Key& key) {
        if (key.physical == GLFW_KEY_W)
        {
            this->Z = false;
        }
        if (key.physical == GLFW_KEY_A)
        {
            this->Q = false;
        }
        if (key.physical == GLFW_KEY_S)
        {
            this->S = false;
        }
        if (key.physical == GLFW_KEY_D)
        {
            this->D = false;
        }
    };
}

void GLContext::draw(p6::Context& ctx)
{
    if (this->Z)
    {
        this->m_renderer.m_scene.m_character.moveFront(ctx);
    }
    if (this->S)
    {
        this->m_renderer.m_scene.m_character.moveBack(ctx);
    }
    if (this->Q)
    {
        this->m_renderer.m_scene.m_character.moveLeft(ctx);
    }
    if (this->D)
    {
        this->m_renderer.m_scene.m_character.moveRight(ctx);
    }
    this->m_renderer.render(ctx, this->m_shadowMap);
}

void GLContext::deleteBuffers()
{
    this->m_renderer.deleteBuffers();
}