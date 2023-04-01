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
    Boid boid;
    Boid boid1({-0.5, 0.5, -6});
    Boid boid2({0.5, 0.3, -2});

    Boid boid3({-1, -0.5, -8});

    std::vector<Boid> boids;
    boids.push_back(boid);
    boids.push_back(boid1);
    boids.push_back(boid2);
    boids.push_back(boid3);

    GLContext glContext(boids);
    glContext.setShader("shaders/3D.vs.glsl", "shaders/red.fs.glsl");

    glContext.initTransformations(ctx);

    // Declare your infinite update loop.
    ctx.update = [&]() {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        /*********************************
         * HERE SHOULD COME THE RENDERING CODE
         *********************************/
        glContext.drawBoids();
    };
    glContext.deleteBuffers();
    //  Should be done last. It starts the infinite loop.
    ctx.start();
}