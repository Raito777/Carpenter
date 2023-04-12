#pragma once

#include <stddef.h>
#include <vector>
#include "Boid.hpp"
#include "Environment.hpp"
#include "FreeflyCamera.hpp"
#include "GLVao.hpp"
#include "GLVbo.hpp"
#include "OBJLoader.hpp"
#include "Scene.hpp"
#include "ShadowMapFBO.hpp"
#include "TrackballCamera.hpp"
#include "glimac/common.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/fwd.hpp"
#include "p6/p6.h"

struct ShadowProgram {
    p6::Shader m_Program;
    GLint      uMVPLight;
    GLint      uMMatrix;
    GLint      uLightPos;
    GLint      ufar_plane;

    ShadowProgram()
        : m_Program(p6::load_shader("shaders/shadow.vs.glsl", "shaders/shadow.fs.glsl"))
    {
        this->uMVPLight  = glGetUniformLocation(m_Program.id(), "uMVPLight");
        this->uMMatrix   = glGetUniformLocation(m_Program.id(), "uMMatrix");
        this->uLightPos  = glGetUniformLocation(m_Program.id(), "uLightPos");
        this->ufar_plane = glGetUniformLocation(m_Program.id(), "ufar_plane");
    }
};

struct CameraDirection {
    GLenum CubemapFace;
    float  theta;
    float  phi;
};

class Renderer {
private:
    GLVbo m_vboBoids;
    GLVbo m_vboEnvironment;
    GLVbo m_vboCharacter;

    GLVao m_vaoBoids;
    GLVao m_vaoEnvironment;
    GLVao m_vaoCharacter;

    p6::Shader                   m_shader     = p6::load_shader("shaders/3D.vs.glsl", "shaders/normals.fs.glsl");
    float                        m_near       = 0.1f;
    float                        m_far        = 80.0f;
    glm::mat4                    m_shadowProj = glm::perspective(glm::radians(90.f), 4096 / static_cast<float>(4096), m_near, m_far);
    std::vector<glm::mat4>       m_shadowTransforms;
    ShadowProgram                m_shadowProgram;
    std::vector<CameraDirection> m_cameraDirections;
    FreeflyCamera                m_viewMatrixLight = FreeflyCamera();

    void renderLights(p6::Context& ctx);
    void renderCamera(p6::Context& ctx);
    void renderBoids(p6::Context& ctx, ShadowCubeMapFBO& shadowMap);
    void renderBoidsShadows(p6::Context& ctx);
    void renderEnvironment(p6::Context& ctx, ShadowCubeMapFBO& shadowMap);
    void renderEnvironmentShadows(p6::Context& ctx);
    void renderCharacter(p6::Context& ctx, ShadowCubeMapFBO& shadowMap);
    void renderCharacterShadows(p6::Context& ctx);

public:
    std::vector<Boid> m_boidsContainer;
    Scene             m_scene;

    Renderer(){};
    Renderer(p6::Context& ctx, std::vector<Boid> boidsContainer);

    void render(p6::Context& ctx, ShadowCubeMapFBO& shadowMap);
    void deleteBuffers();
};
