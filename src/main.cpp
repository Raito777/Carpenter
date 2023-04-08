#include <random>
#include <vector>
#include "Boid.hpp"
#include "GLContext.hpp"
#include "OBJLoader.hpp"
#include "Renderer.hpp"
#include "Scene.hpp"
#include "glimac/common.hpp"
#include "glimac/cone_vertices.hpp"
#include "glm/ext.hpp"
#include "imgui.h"
#include "p6/p6.h"

int windowWidth  = 1280;
int windowHeight = 720;

int main()
{
    auto ctx = p6::Context{{windowWidth, windowHeight, "Carpenter"}};
    ctx.maximize_window();

    /*********************************
     * HERE SHOULD COME THE INITIALIZATION CODE
     *********************************/
    std::vector<Boid> boids;

    for (size_t i = 0; i < 50; i++)
    {
        glm::vec3 position{p6::random::number(-ctx.aspect_ratio(), ctx.aspect_ratio()), p6::random::number(-1, 1), p6::random::number(-5, 0)};
        // glm::vec3 position{0, 0, -3};

        float size = 0.0005f;
        Boid  boid(position, size);
        boids.push_back(boid);
    }

    GLContext glContext(ctx, boids);

    // Declare your infinite update loop.

    ctx.update = [&]() {
        glClearColor(0.5f, 0.5f, 0.5f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glfwSetInputMode(ctx.underlying_glfw_window(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);

        glContext.draw(ctx);
    };
    glContext.deleteBuffers();
    //  Should be done last. It starts the infinite loop.
    ctx.start();

    return 0;
}