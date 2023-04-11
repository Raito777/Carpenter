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

class ShadowCubeMapFBO {
public:
    ShadowCubeMapFBO();
    ~ShadowCubeMapFBO();
    bool Init(unsigned int size);
    void BindForWriting(GLenum CubeFace);
    void BindForReading(GLenum TextureUnit);

private:
    unsigned int m_size          = 0;
    GLuint       m_fbo           = 0;
    GLuint       m_shadowCubeMap = 0;
};
