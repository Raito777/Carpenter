#pragma once

#include <stdlib.h>
#include <random>
#include <vector>
#include "Boid.hpp"
#include "glimac/common.hpp"
#include "glm/ext.hpp"
#include "glm/fwd.hpp"
#include "glm/geometric.hpp"
#include "p6/p6.h"

struct transformations {
    glm::mat4 ProjMatrix;
    glm::mat4 MVMatrix;
    glm::mat4 NormalMatrix;
};
struct shaderGlints {
    GLint uMVPMatrix;
    GLint uMVMatrix;
    GLint uNormalMatrix;
};

class GLContext {
private:
    std::vector<Boid>                m_boidsContainer;
    transformations                  m_transformations;
    std::vector<glimac::ShapeVertex> m_BoidVertices;
    shaderGlints                     m_shaderGlints;
    GLuint                           vbo;
    GLuint                           vao;

public:
    GLContext(const std::vector<Boid>& boidsContainer);

    void initBuffers();
    void initTransformations(p6::Context& ctx);
    void setBoidsVertices(const std::vector<glimac::ShapeVertex>& vertices);
    void setShaderGlints(const int& shaderId);
    void drawBoids();
    void deleteBuffers();

    // const p6::Shader GLContext::loadShaders();
};