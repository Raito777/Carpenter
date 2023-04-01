#include "GLContext.hpp"
#include <vcruntime.h>
#include <iostream>
#include <string_view>
#include <vector>
#include "Boid.hpp"
#include "glimac/common.hpp"
#include "glimac/cone_vertices.hpp"
#include "glm/ext.hpp"
#include "p6/p6.h"

GLContext::GLContext(const std::vector<Boid>& boidsContainer)
    : m_boidsContainer(boidsContainer)
{
    glEnable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);

    this->m_shader.use();

    this->setShaderGlints();

    this->m_vbo = GLVbo();

    this->setBoidsVertices(glimac::cone_vertices(1.f, 0.5f, 16, 32));
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
}

void GLContext::initTransformations(p6::Context& ctx)
{
    this->m_transformations.ProjMatrix   = glm::perspective(glm::radians(70.f), (GLfloat)ctx.aspect_ratio(), 0.1f, 100.f);
    this->m_transformations.MVMatrix     = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, -5));
    this->m_transformations.NormalMatrix = glm::transpose(glm::inverse(this->m_transformations.ProjMatrix));
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
}
void GLContext::drawBoids()
{
    this->m_shader.use();
    this->m_vao.bind();
    for (size_t i = 0; i < this->m_boidsContainer.size(); i++)
    {
        this->m_transformations.MVMatrix = glm::translate(glm::mat4(1.0f), this->m_boidsContainer[i].m_position);
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

// const p6::Shader GLContext::loadShaders()
// {
//     const p6::Shader shader = p6::load_shader(
//         "shaders/3D.vs.glsl",
//         "shaders/normals.fs.glsl"
//     );
//     return shader;
// }
