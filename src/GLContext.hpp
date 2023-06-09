#pragma once

#include <stdlib.h>
#include <random>
#include <string_view>
#include <vector>
#include "Boid.hpp"
#include "Environment.hpp"
#include "GLVao.hpp"
#include "GLVbo.hpp"
#include "Renderer.hpp"
#include "ShadowMapFBO.hpp"
#include "TrackballCamera.hpp"
#include "glimac/common.hpp"
#include "glm/ext.hpp"
#include "glm/fwd.hpp"
#include "glm/geometric.hpp"
#include "p6/p6.h"

class GLContext {
private:
    Renderer m_renderer;
    bool     Z      = false;
    bool     Q      = false;
    bool     S      = false;
    bool     D      = false;
    bool     ESCAPE = false;

    float m_guiLightAdjustment = 70.f;
    float m_lightColor[3]      = {1, 0.3, 0.3};
    int   m_boidNumModel       = 3;
    int   m_environmentModel   = 1;

    float m_globalSpeedFactor      = 0.05;
    float m_globalDetectionFactor  = 1.0;
    float m_globalAvoidanceFactor  = 1.0;
    float m_globalSeparationFactor = 1.2;
    float m_globalAlignmentFactor  = 0.010;
    float m_globalCohesionFactor   = 0.002;

public:
    GLContext(p6::Context& ctx);

    ShadowCubeMapFBO m_shadowMap;
    void             draw(p6::Context& ctx);
    void             deleteBuffers();
    void             updateSettings();
};