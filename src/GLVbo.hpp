#pragma once

#include "p6/p6.h"

class GLVbo {
private:
public:
    GLuint vbo;
    GLVbo();
    void bind();
    void unBind();
    void deleteVbo();
};