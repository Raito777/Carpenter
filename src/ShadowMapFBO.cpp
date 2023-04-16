#include "ShadowMapFBO.hpp"
#include <iostream>

ShadowCubeMapFBO::ShadowCubeMapFBO() = default;

ShadowCubeMapFBO::~ShadowCubeMapFBO()
{
    glDeleteFramebuffers(1, &m_fbo);
    glDeleteTextures(1, &m_shadowCubeMap);
    // // glDeleteTextures(1, &m_depth);
}

bool ShadowCubeMapFBO::Init(unsigned int size)
{
    m_size = size;

    // create the cube map
    glGenTextures(1, &m_shadowCubeMap);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_shadowCubeMap);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    for (size_t i = 0; i < 6; i++)
    {
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, m_size, m_size, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    }

    // Create the FBO
    glGenFramebuffers(1, &m_fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, m_shadowCubeMap, 0);

    // Disable writes to the color buffer
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);

    GLenum Status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

    if (Status != GL_FRAMEBUFFER_COMPLETE)
    {
        std::cerr << "FB error status : " << Status << std::endl;
        return false;
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

    return true;
}

void ShadowCubeMapFBO::BindForWriting(GLenum CubeFace)
{
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_fbo);
    glViewport(0, 0, m_size, m_size);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, CubeFace, m_shadowCubeMap, 0);
    // glDrawBuffer(GL_DEPTH_ATTACHMENT);
}

void ShadowCubeMapFBO::BindForReading(GLenum TextureUnit)
{
    glActiveTexture(TextureUnit);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_shadowCubeMap);
}
