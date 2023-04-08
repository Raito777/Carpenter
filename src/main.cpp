#include <random>
#include <vector>
#include "Boid.hpp"
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
    Environment environment(5, 2.5, 2.5);

    std::vector<Boid> boids;

    for (size_t i = 0; i < 1000; i++)
    {
        glm::vec3 position{p6::random::number(-environment.m_sizeX, environment.m_sizeX), p6::random::number(-environment.m_sizeY, environment.m_sizeY), p6::random::number(-environment.m_sizeZ, environment.m_sizeZ)};
        float     size = 0.001f;
        // float size = 0.1f;
        Boid boid(position, size);
        boids.push_back(boid);
    }

    GLContext glContext(ctx, boids, environment);

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