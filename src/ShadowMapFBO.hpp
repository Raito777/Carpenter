#pragma once

#include <stddef.h>
#include <vector>
#include "Boid.hpp"
#include "Environment.hpp"
#include "GLVao.hpp"
#include "GLVbo.hpp"
#include "OBJLoader.hpp"
#include "Scene.hpp"
#include "TrackballCamera.hpp"
#include "glimac/common.hpp"
#include "glm/fwd.hpp"
#include "p6/p6.h"

class ShadowMapFBO {
public:
    ShadowMapFBO();

    ~ShadowMapFBO();

    void deleteBuffers();

    bool Init(unsigned int WindowWidth, unsigned int WindowHeight);

    void BindForWriting();

    void BindForReading(GLenum TextureUnit);

private:
    unsigned int m_width     = 0;
    unsigned int m_height    = 0;
    GLuint       m_fbo       = 0;
    GLuint       m_shadowMap = 0;
};