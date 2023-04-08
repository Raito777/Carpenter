#include "Renderer.hpp"

Renderer::Renderer(p6::Context& ctx, std::vector<Boid> boidsContainer, Environment environment)
{
    this->m_boidsContainer = boidsContainer;

    this->m_environment = environment;

    this->m_shader = p6::load_shader("shaders/3D.vs.glsl", "shaders/pointlight.fs.glsl");
    this->m_shader.use();

    this->m_scene = Scene(ctx, this->m_shader.id());

    this->m_vboBoids.bind();
    glBufferData(GL_ARRAY_BUFFER, this->m_scene.m_boidModel.size() * sizeof(glimac::ShapeVertex), this->m_scene.m_boidModel.data(), GL_STATIC_DRAW);
    this->m_vboBoids.unBind();

    this->m_vboEnvironment.bind();
    glBufferData(GL_ARRAY_BUFFER, this->m_scene.m_environmentModel.size() * sizeof(glimac::ShapeVertex), this->m_scene.m_environmentModel.data(), GL_STATIC_DRAW);
    this->m_vboEnvironment.unBind();

    static constexpr GLuint VERTEX_ATTR_POSITION  = 0;
    static constexpr GLuint VERTEX_ATTR_COLOR     = 1;
    static constexpr GLuint VERTEX_ATTR_TEXCOORDS = 2;

    this->m_vaoBoids.bind();

    glEnableVertexAttribArray(VERTEX_ATTR_POSITION);
    glEnableVertexAttribArray(VERTEX_ATTR_COLOR);
    glEnableVertexAttribArray(VERTEX_ATTR_TEXCOORDS);

    this->m_vboBoids.bind();

    glVertexAttribPointer(VERTEX_ATTR_POSITION, 3, GL_FLOAT, GL_FALSE, sizeof(glimac::ShapeVertex), (const GLvoid*)(offsetof(glimac::ShapeVertex, position)));
    glVertexAttribPointer(VERTEX_ATTR_COLOR, 3, GL_FLOAT, GL_FALSE, sizeof(glimac::ShapeVertex), (const GLvoid*)(offsetof(glimac::ShapeVertex, normal)));
    glVertexAttribPointer(VERTEX_ATTR_TEXCOORDS, 2, GL_FLOAT, GL_FALSE, sizeof(glimac::ShapeVertex), (const GLvoid*)(offsetof(glimac::ShapeVertex, texCoords)));

    this->m_vboBoids.unBind();
    this->m_vaoBoids.unBind();

    this->m_vaoEnvironment.bind();

    glEnableVertexAttribArray(VERTEX_ATTR_POSITION);
    glEnableVertexAttribArray(VERTEX_ATTR_COLOR);
    glEnableVertexAttribArray(VERTEX_ATTR_TEXCOORDS);

    this->m_vboEnvironment.bind();

    glVertexAttribPointer(VERTEX_ATTR_POSITION, 3, GL_FLOAT, GL_FALSE, sizeof(glimac::ShapeVertex), (const GLvoid*)(offsetof(glimac::ShapeVertex, position)));
    glVertexAttribPointer(VERTEX_ATTR_COLOR, 3, GL_FLOAT, GL_FALSE, sizeof(glimac::ShapeVertex), (const GLvoid*)(offsetof(glimac::ShapeVertex, normal)));
    glVertexAttribPointer(VERTEX_ATTR_TEXCOORDS, 2, GL_FLOAT, GL_FALSE, sizeof(glimac::ShapeVertex), (const GLvoid*)(offsetof(glimac::ShapeVertex, texCoords)));

    this->m_vboEnvironment.unBind();
    this->m_vaoEnvironment.unBind();

    this->m_shadowMap.Init(this->m_scene.shadow_width, this->m_scene.shadow_height);

    glEnable(GL_POLYGON_OFFSET_FILL);
    float factor = 1.f;
    float units  = 1.f;
    glPolygonOffset(factor, units);
}

void Renderer::render(p6::Context& ctx)
{
    this->m_shadowMap.BindForWriting();

    glClear(GL_DEPTH_BUFFER_BIT);

    this->m_shadowShader.use();

    this->m_shader.use();
    this->renderCamera(ctx);
    this->renderLights(ctx);
    this->m_vaoEnvironment.bind();
    this->renderEnvironment(ctx);
    this->m_vaoEnvironment.unBind();
    this->m_vaoBoids.bind();
    this->renderBoids(ctx);
    this->m_vaoBoids.unBind();
}

void Renderer::renderLights(p6::Context& ctx)
{
    glm::vec3 uMVLightsPos;

    this->m_scene.m_pointLight._lightPos = glm::vec3(glm::sin(ctx.time()) * 3, 0, -0.5);
    uMVLightsPos                         = glm::vec3(this->m_scene.m_camera.getViewMatrix() * glm::vec4(this->m_scene.m_pointLight._lightPos, 1));
    glUniform3fv(this->m_scene.m_pointLight.uLightPos_vs, 1, glm::value_ptr(uMVLightsPos));
    glUniform3fv(this->m_scene.m_pointLight.uLightIntensity, 1, glm::value_ptr(this->m_scene.m_pointLight._uLightIntensity));
    glUniform3fv(this->m_scene.m_pointLight.uAmbient, 1, glm::value_ptr(this->m_scene.m_pointLight._uAmbient));

    glUniform3fv(this->m_scene.m_dirLight.uLightDir_vs, 1, glm::value_ptr(this->m_scene.m_dirLight._lightDir));
    glUniform3fv(this->m_scene.m_dirLight.uLightIntensity, 1, glm::value_ptr(this->m_scene.m_dirLight._uLightIntensity));
}

void Renderer::renderCamera(p6::Context& ctx)
{
    float movementStrength = 5000.f * ctx.delta_time();
    this->m_scene.m_camera.rotateLeft(ctx.mouse_delta().y * movementStrength);
    this->m_scene.m_camera.rotateUp(ctx.mouse_delta().x * movementStrength);
}

void Renderer::renderBoids(p6::Context& ctx)
{
    glUniform3fv(this->m_scene.m_boidLightTexture.uKd, 1, glm::value_ptr(this->m_scene.m_boidLightTexture._uKd[0]));
    glUniform3fv(this->m_scene.m_boidLightTexture.uKs, 1, glm::value_ptr(this->m_scene.m_boidLightTexture._uKs[0]));
    glUniform1f(this->m_scene.m_boidLightTexture.uShininess, this->m_scene.m_boidLightTexture._uShininess[0]);

    for (size_t i = 0; i < this->m_boidsContainer.size(); i++)
    {
        this->m_boidsContainer[i].moove(ctx);
        this->m_boidsContainer[i].checkBorder(ctx, this->m_environment);
        // CALCUL LA ROTATION DU BOIDS EN FONCTION DE SA DIRECTION
        glm::vec3 direction = glm::normalize(this->m_boidsContainer[i].m_direction);
        glm::vec3 up        = glm::vec3(0.0f, 0.0f, 1.0f); // boids orientés vers le haut par défaut
        glm::vec3 axis      = glm::normalize(glm::cross(up, direction));
        float     angle     = glm::radians(glm::degrees(glm::acos(glm::dot(up, direction))));

        this->m_scene.m_boidsTransformations.MVMatrix = glm::translate(this->m_scene.m_camera.getViewMatrix(), this->m_boidsContainer[i].m_position);
        this->m_scene.m_boidsTransformations.MVMatrix = glm::rotate(this->m_scene.m_boidsTransformations.MVMatrix, angle, axis);
        this->m_scene.m_boidsTransformations.MVMatrix = glm::scale(this->m_scene.m_boidsTransformations.MVMatrix, glm::vec3(this->m_boidsContainer[i].m_size, this->m_boidsContainer[i].m_size, this->m_boidsContainer[i].m_size));

        glUniformMatrix4fv(this->m_scene.m_dirLight.uMVPLight, 1, GL_FALSE, glm::value_ptr(this->shadowOrthoProjMat * this->m_scene.m_boidsTransformations.MVMatrix));

        glUniformMatrix4fv(this->m_scene.m_boidsTransformations.uMVPMatrix, 1, GL_FALSE, glm::value_ptr(this->m_scene.m_boidsTransformations.ProjMatrix * this->m_scene.m_boidsTransformations.MVMatrix));
        glUniformMatrix4fv(this->m_scene.m_boidsTransformations.uMVMatrix, 1, GL_FALSE, glm::value_ptr(this->m_scene.m_boidsTransformations.MVMatrix));
        glUniformMatrix4fv(this->m_scene.m_boidsTransformations.uNormalMatrix, 1, GL_FALSE, glm::value_ptr(this->m_scene.m_boidsTransformations.NormalMatrix));

        glDrawArrays(GL_TRIANGLES, 0, this->m_scene.m_boidModel.size());
    }
}

void Renderer::renderEnvironment(p6::Context& ctx)
{
    this->m_scene.m_environmentTransformations.MVMatrix = glm::translate(this->m_scene.m_camera.getViewMatrix(), this->m_environment.m_position);
    this->m_scene.m_environmentTransformations.MVMatrix = glm::rotate(this->m_scene.m_environmentTransformations.MVMatrix, glm::radians(90.f), glm::vec3(1.f, 0.f, 0.f));

    this->m_scene.m_environmentTransformations.MVMatrix = glm::scale(this->m_scene.m_environmentTransformations.MVMatrix, glm::vec3(this->m_environment.m_sizeX, this->m_environment.m_sizeY, this->m_environment.m_sizeZ));

    glUniform3fv(this->m_scene.m_environmentLightTexture.uKd, 1, glm::value_ptr(this->m_scene.m_environmentLightTexture._uKd[0]));
    glUniform3fv(this->m_scene.m_environmentLightTexture.uKs, 1, glm::value_ptr(this->m_scene.m_environmentLightTexture._uKs[0]));
    glUniform1f(this->m_scene.m_environmentLightTexture.uShininess, this->m_scene.m_environmentLightTexture._uShininess[0]);

    glUniformMatrix4fv(this->m_scene.m_environmentTransformations.uMVPMatrix, 1, GL_FALSE, glm::value_ptr(this->m_scene.m_environmentTransformations.ProjMatrix * this->m_scene.m_environmentTransformations.MVMatrix));
    glUniformMatrix4fv(this->m_scene.m_environmentTransformations.uMVMatrix, 1, GL_FALSE, glm::value_ptr(this->m_scene.m_environmentTransformations.MVMatrix));
    glUniformMatrix4fv(this->m_scene.m_environmentTransformations.uNormalMatrix, 1, GL_FALSE, glm::value_ptr(this->m_scene.m_environmentTransformations.NormalMatrix));

    glDrawArrays(GL_TRIANGLES, 0, this->m_scene.m_environmentModel.size());
}

void Renderer::deleteBuffers()
{
    this->m_vboBoids.deleteVbo();
    this->m_vboEnvironment.deleteVbo();

    this->m_vaoBoids.deleteVao();
    this->m_vaoEnvironment.deleteVao();
}