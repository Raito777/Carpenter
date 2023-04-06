#include "GLContext.hpp"
#include <vcruntime.h>
#include <glm/gtx/vector_angle.hpp>
#include <iostream>
#include <string_view>
#include <vector>
#include "Boid.hpp"
#include "OBJLoader.hpp"
#include "glimac/common.hpp"
#include "glimac/cone_vertices.hpp"
#include "glimac/sphere_vertices.hpp"
#include "glm/ext.hpp"
#include "p6/p6.h"

GLContext::GLContext(const std::vector<Boid>& boidsContainer, p6::Context& ctx)
    : m_boidsContainer(boidsContainer)
{
    glEnable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);

    this->m_shader.use();

    this->setShaderGlints();

    this->m_vbo = GLVbo();

    // this->setBoidsVertices(glimac::cone_vertices(1.f, 0.5f, 16, 32));
    this->setBoidsVertices(loadOBJ("./assets/models/bat2.obj"));
    glBufferData(GL_ARRAY_BUFFER, this->m_BoidVertices.size() * sizeof(glimac::ShapeVertex), this->m_BoidVertices.data(), GL_STATIC_DRAW);

    this->m_vbo.unBind();

    this->m_vao = GLVao();

    static constexpr GLuint VERTEX_ATTR_POSITION  = 0;
    static constexpr GLuint VERTEX_ATTR_COLOR     = 1;
    static constexpr GLuint VERTEX_ATTR_TEXCOORDS = 2;

    glEnableVertexAttribArray(VERTEX_ATTR_POSITION);
    glEnableVertexAttribArray(VERTEX_ATTR_COLOR);
    glEnableVertexAttribArray(VERTEX_ATTR_TEXCOORDS);

    this->m_vbo.bind();

    glVertexAttribPointer(VERTEX_ATTR_POSITION, 3, GL_FLOAT, GL_FALSE, sizeof(glimac::ShapeVertex), (const GLvoid*)(offsetof(glimac::ShapeVertex, position)));
    glVertexAttribPointer(VERTEX_ATTR_COLOR, 3, GL_FLOAT, GL_FALSE, sizeof(glimac::ShapeVertex), (const GLvoid*)(offsetof(glimac::ShapeVertex, normal)));
    glVertexAttribPointer(VERTEX_ATTR_TEXCOORDS, 2, GL_FLOAT, GL_FALSE, sizeof(glimac::ShapeVertex), (const GLvoid*)(offsetof(glimac::ShapeVertex, texCoords)));

    this->m_vbo.unBind();
    this->m_vao.unBind();

    this->initTransformations(ctx);

    for (size_t i = 0; i < this->m_boidsContainer.size() + 1; i++)
    {
        this->m_lightSetup._uKd.push_back(glm::vec3(1.f, 0.1f, 0.1f));
        this->m_lightSetup._uKs.push_back(glm::vec3(1.f, 0.1f, 0.1f));
        this->m_lightSetup._uLightIntensity.push_back(glm::vec3(glm::linearRand(0.f, 1.0f), glm::linearRand(0.f, 1.0f), glm::linearRand(0.f, 1.0f)));
        this->m_lightSetup._uShininess.push_back(0.4f);
    }
    this->m_lightSetup.light = glm::vec3(0, 0, -3);

    ctx.mouse_scrolled = [&](p6::MouseScroll data) {
        this->m_camera.moveFront(data.dy);
    };
}

void GLContext::initTransformations(p6::Context& ctx)
{
    this->m_transformations.ProjMatrix   = glm::perspective(glm::radians(70.f), (GLfloat)ctx.aspect_ratio(), 0.1f, 100.f);
    this->m_transformations.MVMatrix     = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
    this->m_transformations.NormalMatrix = glm::transpose(glm::inverse(this->m_transformations.MVMatrix));
}

void GLContext::setBoidsVertices(const std::vector<glimac::ShapeVertex>& vertices)
{
    this->m_BoidVertices = vertices;
}

void GLContext::setShaderGlints()
{
    this->m_shaderGlints.uMVPMatrix    = glGetUniformLocation(this->m_shader.id(), "uMVPMatrix");
    this->m_shaderGlints.uMVMatrix     = glGetUniformLocation(this->m_shader.id(), "uMVMatrix");
    this->m_shaderGlints.uNormalMatrix = glGetUniformLocation(this->m_shader.id(), "uNormalMatrix");
    this->m_lightSetup.uAmbient        = glGetUniformLocation(m_shader.id(), "uKa");
    this->m_lightSetup.uKd             = glGetUniformLocation(m_shader.id(), "uKd");
    this->m_lightSetup.uKs             = glGetUniformLocation(m_shader.id(), "uKs");
    this->m_lightSetup.uShininess      = glGetUniformLocation(m_shader.id(), "uShininess");
    this->m_lightSetup.uLightPos_vs    = glGetUniformLocation(m_shader.id(), "uLightPos_vs");
    this->m_lightSetup.uLightIntensity = glGetUniformLocation(m_shader.id(), "uLightIntensity");
}
// class renderer qui permet de choisir quoi rendre
// Struct scene avec les lumières les objets et leurs transformations et on donne ça au renderer
void GLContext::drawBoids(p6::Context& ctx)
{
    // this->m_camera.moveFront(0.1f);
    float movementStrength = 30.f;
    this->m_camera.rotateLeft(ctx.mouse_delta().y * movementStrength);
    this->m_camera.rotateUp(ctx.mouse_delta().x * movementStrength);

    ctx.mouse() = {0, 0};
    this->m_shader.use();
    this->m_vao.bind();

    glm::vec3 uMVLightPos = glm::vec3(this->m_camera.getViewMatrix() * glm::vec4(this->m_lightSetup.light, 1));

    glUniform3fv(this->m_lightSetup.uLightPos_vs, 1, glm::value_ptr(uMVLightPos));
    glUniform3fv(this->m_lightSetup.uLightIntensity, 1, glm::value_ptr(glm::vec3(3.f, 3.f, 3.f)));
    glUniform3fv(this->m_lightSetup.uAmbient, 1, glm::value_ptr(glm::vec3(0.1f, 0.1f, 0.1f)));

    for (size_t i = 0; i < this->m_boidsContainer.size(); i++)
    {
        this->m_boidsContainer[i].moove();
        this->m_boidsContainer[i].checkBorder(ctx);
        // CALCUL LA ROTATION DU BOIDS EN FONCTION DE SA DIRECTION
        glm::vec3 direction = glm::normalize(this->m_boidsContainer[i].m_direction);
        glm::vec3 up        = glm::vec3(0.0f, 0.0f, 1.0f); // boids orientés vers le haut par défaut
        glm::vec3 axis      = glm::normalize(glm::cross(up, direction));
        float     angle     = glm::radians(glm::degrees(glm::acos(glm::dot(up, direction))));

        this->m_transformations.MVMatrix = glm::translate(this->m_camera.getViewMatrix(), this->m_boidsContainer[i].m_position);
        this->m_transformations.MVMatrix = glm::rotate(this->m_transformations.MVMatrix, angle, axis);
        this->m_transformations.MVMatrix = glm::scale(this->m_transformations.MVMatrix, glm::vec3(this->m_boidsContainer[i].m_size, this->m_boidsContainer[i].m_size, this->m_boidsContainer[i].m_size));

        //-------------------------------------//
        glUniform3fv(this->m_lightSetup.uKd, 1, glm::value_ptr(this->m_lightSetup._uKd[i]));
        glUniform3fv(this->m_lightSetup.uKs, 1, glm::value_ptr(this->m_lightSetup._uKs[i]));
        glUniform1f(this->m_lightSetup.uShininess, this->m_lightSetup._uShininess[i]);

        glUniformMatrix4fv(this->m_shaderGlints.uMVPMatrix, 1, GL_FALSE, glm::value_ptr(this->m_transformations.ProjMatrix * this->m_transformations.MVMatrix));
        glUniformMatrix4fv(this->m_shaderGlints.uMVMatrix, 1, GL_FALSE, glm::value_ptr(this->m_transformations.MVMatrix));
        glUniformMatrix4fv(this->m_shaderGlints.uNormalMatrix, 1, GL_FALSE, glm::value_ptr(this->m_transformations.NormalMatrix));
        glDrawArrays(GL_TRIANGLES, 0, this->m_BoidVertices.size());
    }
    this->m_vao.unBind();
}

void GLContext::deleteBuffers()
{
    this->m_vbo.deleteVbo();
    this->m_vao.deleteVao();
}

void GLContext::setShader(const std::string& vertexShaderPath, const std::string& fragmentShaderPath)
{
    this->m_shader = p6::load_shader(vertexShaderPath, fragmentShaderPath);
    this->setShaderGlints();
}

void GLContext::initOBJModels()
{
}