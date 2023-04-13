#include "GLContext.hpp"
#include <vcruntime.h>
#include <glm/gtx/vector_angle.hpp>
#include <iostream>
#include <string_view>
#include <vector>
#include "Boid.hpp"
#include "Environment.hpp"
#include "GLFW/glfw3.h"
#include "OBJLoader.hpp"
#include "Renderer.hpp"
#include "Scene.hpp"
#include "glimac/common.hpp"
#include "glimac/cone_vertices.hpp"
#include "glimac/sphere_vertices.hpp"
#include "glm/ext.hpp"
#include "imgui.h"
#include "p6/p6.h"

GLContext::GLContext(p6::Context& ctx, std::vector<Boid>& boidsContainer)
{
    glEnable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);

    m_shadowMap.Init(2048);

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
        if (key.physical == GLFW_KEY_ESCAPE)
        {
            if (this->ESCAPE)
            {
                this->ESCAPE = false;
            }
            else
            {
                this->ESCAPE = true;
            }
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
    ImGui::Begin("Settings");
    ImGui::ColorEdit3("Color", m_lightColor);
    ImGui::SliderFloat("Light 1 intensity", &this->m_guiLightAdjustment, 0.f, 500.f);
    ImGui::Text("Boid model %d", m_boidNumModel);
    ImGui::RadioButton("Model boid 1", &m_boidNumModel, 1);
    ImGui::SameLine();
    ImGui::RadioButton("Model boid 2", &m_boidNumModel, 2);
    ImGui::SameLine();
    ImGui::RadioButton("Model boid 3", &m_boidNumModel, 3);

    ImGui::Text("Environment model %d", m_environmentModel);
    ImGui::RadioButton("Model env 1", &m_environmentModel, 1);
    ImGui::SameLine();
    ImGui::RadioButton("Model env 2", &m_environmentModel, 2);

    ImGui::End();

    if (!this->ESCAPE)
    {
        glfwSetInputMode(ctx.underlying_glfw_window(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }
    else
    {
        glfwSetInputMode(ctx.underlying_glfw_window(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        // setting the variables that are in gui
        this->m_renderer.m_scene.setBoidModel(this->m_boidNumModel);
        this->m_renderer.updateBoidVbo();
        this->m_renderer.m_scene.setEnvironmentModel(this->m_environmentModel);
        this->m_renderer.updateEnvironmentVbo();

        this->m_renderer.m_scene.setLight1Intensity(this->m_guiLightAdjustment);
        this->m_renderer.m_scene.setLightColor(this->m_lightColor, this->m_guiLightAdjustment);
    }

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