#pragma once

#include "p6/p6.h"

class GLVao {
private:
public:
    GLuint vao;
    GLVao();
    void bind();
    void unBind();
    void deleteVao();
};