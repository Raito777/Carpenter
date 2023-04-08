#include "GLVbo.hpp"

GLVbo::GLVbo()
{
    glGenBuffers(1, &this->vbo);
}

void GLVbo::bind()
{
    glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
}

void GLVbo::unBind()
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void GLVbo::deleteVbo()
{
    glDeleteBuffers(0, &this->vbo);
}