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
    bool     Z = false;
    bool     Q = false;
    bool     S = false;
    bool     D = false;

public:
    GLContext(p6::Context& ctx, std::vector<Boid>& boidsContainer);

    ShadowCubeMapFBO m_shadowMap;
    void             draw(p6::Context& ctx);
    void             deleteBuffers();

    // const p6::Shader GLContext::loadShaders();
};