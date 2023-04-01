#include "GLVao.hpp"

GLVao::GLVao()
{
    glGenVertexArrays(1, &this->vao);
    this->bind();
}

void GLVao::bind()
{
    glBindVertexArray(this->vao);
}

void GLVao::unBind()
{
    glBindVertexArray(0);
}

void GLVao::deleteVao()
{
    glDeleteVertexArrays(0, &this->vao);
}