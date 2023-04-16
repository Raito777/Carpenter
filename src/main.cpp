#include <vector>
#include "Boid.hpp"
#include "Character.hpp"
#include "Environment.hpp"
#include "GLContext.hpp"
#include "OBJLoader.hpp"
#include "Renderer.hpp"
#include "Scene.hpp"
#include "ShadowMapFBO.hpp"
#include "glimac/common.hpp"
#include "glimac/cone_vertices.hpp"
#include "glm/ext.hpp"
#include "imgui.h"
#include "p6/p6.h"

int const windowWidth  = 1280;
int const windowHeight = 720;

int main()
{
    auto ctx = p6::Context{{windowWidth, windowHeight, "Carpenter"}};
    ctx.maximize_window();

    /*********************************
     * HERE SHOULD COME THE INITIALIZATION CODE
     *********************************/

    GLContext glContext(ctx);

    // Declare your infinite update loop.

    ctx.update = [&]() {
        glContext.draw(ctx);
    };

    glContext.deleteBuffers();
    //  Should be done last. It starts the infinite loop.
    ctx.start();

    return 0;
}