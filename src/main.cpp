#include <random>
#include <vector>
#include "Boid.hpp"
#include "GLContext.hpp"
#include "glimac/common.hpp"
#include "glimac/cone_vertices.hpp"
#include "glm/ext.hpp"
#include "p6/p6.h"

int main()
{
    auto ctx = p6::Context{{1280, 720, "TP3 EX1"}};
    ctx.maximize_window();

    /*********************************
     * HERE SHOULD COME THE INITIALIZATION CODE
     *********************************/
    std::vector<Boid> boids;

    for (size_t i = 0; i < 100; i++)
    {
        glm::vec3 position{p6::random::number(-ctx.aspect_ratio(), ctx.aspect_ratio()), p6::random::number(-1, 1), p6::random::number(-10, 0)};
        Boid      boid(position);
        boids.push_back(boid);
    }
    Boid boid1({-0.5, 0.5, -1});
    Boid boid2({0.5, 0.3, -2});
    Boid boid3({-1, -0.5, -8});
    boids.push_back(boid1);
    boids.push_back(boid2);
    boids.push_back(boid3);

    GLContext glContext(boids);
    float     movementStrength = 5.f;

    ctx.mouse_scrolled = [&](p6::MouseScroll data) {
        glContext.m_camera.moveFront(data.dy);
    };

    glContext.setShader("shaders/3D.vs.glsl", "shaders/pointlight.fs.glsl");

    glContext.initTransformations(ctx);

    // Declare your infinite update loop.
    ctx.update = [&]() {
        glClearColor(0.1f, 0.1f, 0.1f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glContext.drawBoids(ctx);
    };
    glContext.deleteBuffers();
    //  Should be done last. It starts the infinite loop.
    ctx.start();
}