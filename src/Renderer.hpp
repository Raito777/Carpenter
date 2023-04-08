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
        glBufferData(GL_ARRAY_BUFFER, this->m_scene.m_environmentModel.size() * sizeof(glimac::ShapeVertex), this->m_scene.m_environmentModel.data(), GL_STATIC_DRAW);
        this->m_vbo[1].unBind();
    }

    void render(p6::Context& ctx)
    {
        this->m_shader.use();
        this->renderCamera(ctx);
        this->renderLights();
        this->renderEnvironment(ctx);
        this->renderBoids(ctx);
    }

    void renderLights()
    {
        std::vector<glm::vec3> uMVLightsPos;

        for (size_t i = 0; i < this->m_scene.m_lights.size(); i++)
        {
            uMVLightsPos[i] = glm::vec3(this->m_scene.m_camera.getViewMatrix() * glm::vec4(this->m_scene.m_lights[i]._lightPos, 1));
        }
    }

    void renderCamera(p6::Context& ctx)
    {
        float movementStrength = 30.f;
        this->m_scene.m_camera.rotateLeft(ctx.mouse_delta().y * movementStrength);
        this->m_scene.m_camera.rotateUp(ctx.mouse_delta().x * movementStrength);
    }

    void renderBoids(p6::Context& ctx)
    {
        glUniform3fv(this->m_scene.m_boidLightTexture.uKd, 1, glm::value_ptr(this->m_scene.m_boidLightTexture._uKd[0]));
        glUniform3fv(this->m_scene.m_boidLightTexture.uKs, 1, glm::value_ptr(this->m_scene.m_boidLightTexture._uKs[0]));
        glUniform1f(this->m_scene.m_boidLightTexture.uShininess, this->m_scene.m_boidLightTexture._uShininess[0]);

        for (size_t i = 0; i < this->m_boidsContainer.size(); i++)
        {
            this->m_boidsContainer[i].moove();
            this->m_boidsContainer[i].checkBorder(ctx);
            // CALCUL LA ROTATION DU BOIDS EN FONCTION DE SA DIRECTION
            glm::vec3 direction = glm::normalize(this->m_boidsContainer[i].m_direction);
            glm::vec3 up        = glm::vec3(0.0f, 0.0f, 1.0f); // boids orientés vers le haut par défaut
            glm::vec3 axis      = glm::normalize(glm::cross(up, direction));
            float     angle     = glm::radians(glm::degrees(glm::acos(glm::dot(up, direction))));

            this->m_scene.m_boidsTransformations.MVMatrix = glm::translate(this->m_scene.m_camera.getViewMatrix(), this->m_boidsContainer[i].m_position);
            this->m_scene.m_boidsTransformations.MVMatrix = glm::rotate(this->m_scene.m_boidsTransformations.MVMatrix, angle, axis);
            this->m_scene.m_boidsTransformations.MVMatrix = glm::scale(this->m_scene.m_boidsTransformations.MVMatrix, glm::vec3(this->m_boidsContainer[i].m_size, this->m_boidsContainer[i].m_size, this->m_boidsContainer[i].m_size));

            glUniformMatrix4fv(this->m_scene.m_boidsTransformations.uMVPMatrix, 1, GL_FALSE, glm::value_ptr(this->m_scene.m_boidsTransformations.ProjMatrix * this->m_scene.m_boidsTransformations.MVMatrix));
            glUniformMatrix4fv(this->m_scene.m_boidsTransformations.uMVMatrix, 1, GL_FALSE, glm::value_ptr(this->m_scene.m_boidsTransformations.MVMatrix));
            glUniformMatrix4fv(this->m_scene.m_boidsTransformations.uNormalMatrix, 1, GL_FALSE, glm::value_ptr(this->m_scene.m_boidsTransformations.NormalMatrix));

            glDrawArrays(GL_TRIANGLES, 0, this->m_scene.m_boidModel.size());
        }
    }

    void renderEnvironment(p6::Context& ctx)
    {
        glUniform3fv(this->m_scene.m_environmentLightTexture.uKd, 1, glm::value_ptr(this->m_scene.m_environmentLightTexture._uKd[0]));
        glUniform3fv(this->m_scene.m_environmentLightTexture.uKs, 1, glm::value_ptr(this->m_scene.m_environmentLightTexture._uKs[0]));
        glUniform1f(this->m_scene.m_environmentLightTexture.uShininess, this->m_scene.m_environmentLightTexture._uShininess[0]);

        this->m_scene.m_environmentTransformations.MVMatrix = glm::scale(this->m_scene.m_boidsTransformations.MVMatrix, glm::vec3(0.1f, 0.1f, 0.1f));

        glUniformMatrix4fv(this->m_scene.m_environmentTransformations.uMVPMatrix, 1, GL_FALSE, glm::value_ptr(this->m_scene.m_environmentTransformations.ProjMatrix * this->m_scene.m_environmentTransformations.MVMatrix));
        glUniformMatrix4fv(this->m_scene.m_environmentTransformations.uMVMatrix, 1, GL_FALSE, glm::value_ptr(this->m_scene.m_environmentTransformations.MVMatrix));
        glUniformMatrix4fv(this->m_scene.m_environmentTransformations.uNormalMatrix, 1, GL_FALSE, glm::value_ptr(this->m_scene.m_environmentTransformations.NormalMatrix));

        glDrawArrays(GL_TRIANGLES, 0, this->m_scene.m_environmentModel.size());
    }
};