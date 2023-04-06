#pragma once

#include <stdlib.h>
#include <random>
#include <string_view>
#include <vector>
#include "Boid.hpp"
#include "GLVao.hpp"
#include "GLVbo.hpp"
#include "TrackballCamera.hpp"
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
// std variants pour stocker les lumières positional directional
struct lightSetup {
    GLint uAmbient;
    GLint uKd;
    GLint uKs;
    GLint uShininess;
    GLint uLightPos_vs;
    GLint uLightIntensity;

    std::vector<glm::vec3> _uKd;
    std::vector<glm::vec3> _uKs;
    std::vector<float>     _uShininess;
    std::vector<glm::vec3> _uLightIntensity;
    glm::vec3              light;
};

class GLContext {
private:
    std::vector<Boid>                m_boidsContainer;
    transformations                  m_transformations;
    std::vector<glimac::ShapeVertex> m_BoidVertices;
    shaderGlints                     m_shaderGlints;
    GLVbo                            m_vbo;
    GLVao                            m_vao;
    p6::Shader                       m_shader = p6::load_shader("shaders/3D.vs.glsl", "shaders/red.fs.glsl");
    lightSetup                       m_lightSetup;

public:
    TrackballCamera m_camera;

    GLContext(const std::vector<Boid>& boidsContainer, p6::Context& ctx);
    void initTransformations(p6::Context& ctx);
    void setBoidsVertices(const std::vector<glimac::ShapeVertex>& vertices);
    void setShader(const std::string& vertexShaderPath, const std::string& fragmentShaderPath);
    void setShaderGlints();
    void drawBoids(p6::Context& ctx);
    void deleteBuffers();
    void initOBJModels();

    // const p6::Shader GLContext::loadShaders();
};