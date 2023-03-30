#include "cstddef"
#include "p6/p6.h"

struct Vertex2DColor {
    glm::vec2 m_position;
    glm::vec3 m_color;

    Vertex2DColor(){};
    Vertex2DColor(glm::vec2 position, glm::vec3 color)
        : m_position(position), m_color(color){};
};

int main()
{
    auto ctx = p6::Context{{1280, 720, "Carpenter"}};
    ctx.maximize_window();

    /*********************************
     * HERE SHOULD COME THE INITIALIZATION CODE
     *********************************/
    const p6::Shader shader = p6::load_shader(
        "shaders/triangle.vs.glsl",
        "shaders/triangle.fs.glsl"
    );

    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    Vertex2DColor vertices[] = {
        Vertex2DColor{{-0.5f, -0.5f}, {1.f, 0.f, 0.f}}, // Premier sommet
        Vertex2DColor{{0.5f, -0.5f}, {0.f, 1.f, 0.f}},  // Deuxième sommet
        Vertex2DColor{{0.0f, 0.5f}, {0.f, 0.f, 1.f}}    // Troisième sommet
    };
    glBufferData(GL_ARRAY_BUFFER, 3 * sizeof(Vertex2DColor), vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    static constexpr GLuint VERTEX_ATTR_POSITION = 3;
    static constexpr GLuint VERTEX_ATTR_COLOR    = 8;

    glEnableVertexAttribArray(VERTEX_ATTR_POSITION);
    glEnableVertexAttribArray(VERTEX_ATTR_COLOR);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    glVertexAttribPointer(VERTEX_ATTR_POSITION, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex2DColor), (const GLvoid*)offsetof(Vertex2DColor, m_position));
    glVertexAttribPointer(VERTEX_ATTR_COLOR, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex2DColor), (const GLvoid*)offsetof(Vertex2DColor, m_color));

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);

    // Declare your infinite update loop.
    ctx.update = [&]() {
        glClear(GL_COLOR_BUFFER_BIT);

        /*********************************
         * HERE SHOULD COME THE RENDERING CODE
         *********************************/
        glBindVertexArray(vao);
        shader.use();
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glBindVertexArray(0);
    };
    // Should be done last. It starts the infinite loop.
    ctx.start();
    glDeleteBuffers(0, &vbo);
    glDeleteVertexArrays(0, &vao);
}
