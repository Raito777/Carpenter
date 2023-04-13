#include "Renderer.hpp"
#include <iostream>
#include "glm/fwd.hpp"

Renderer::Renderer(p6::Context& ctx, std::vector<Boid> boidsContainer)
{
    this->m_boidsContainer = boidsContainer;

    this->m_shader = p6::load_shader("shaders/3D.vs.glsl", "shaders/pointlight.fs.glsl");
    this->m_shader.use();

    this->m_scene = Scene(ctx, this->m_shader.id());

    this->m_vboBoids.bind();
    glBufferData(GL_ARRAY_BUFFER, this->m_scene.m_boidModel.size() * sizeof(glimac::ShapeVertex), this->m_scene.m_boidModel.data(), GL_STATIC_DRAW);
    this->m_vboBoids.unBind();

    this->m_vboEnvironment.bind();
    glBufferData(GL_ARRAY_BUFFER, this->m_scene.m_environmentModel.size() * sizeof(glimac::ShapeVertex), this->m_scene.m_environmentModel.data(), GL_STATIC_DRAW);
    this->m_vboEnvironment.unBind();

    this->m_vboCharacter.bind();
    glBufferData(GL_ARRAY_BUFFER, this->m_scene.m_characterModel.size() * sizeof(glimac::ShapeVertex), this->m_scene.m_characterModel.data(), GL_STATIC_DRAW);
    this->m_vboCharacter.unBind();

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

    this->m_vaoCharacter.bind();

    glEnableVertexAttribArray(VERTEX_ATTR_POSITION);
    glEnableVertexAttribArray(VERTEX_ATTR_COLOR);
    glEnableVertexAttribArray(VERTEX_ATTR_TEXCOORDS);

    this->m_vboCharacter.bind();

    glVertexAttribPointer(VERTEX_ATTR_POSITION, 3, GL_FLOAT, GL_FALSE, sizeof(glimac::ShapeVertex), (const GLvoid*)(offsetof(glimac::ShapeVertex, position)));
    glVertexAttribPointer(VERTEX_ATTR_COLOR, 3, GL_FLOAT, GL_FALSE, sizeof(glimac::ShapeVertex), (const GLvoid*)(offsetof(glimac::ShapeVertex, normal)));
    glVertexAttribPointer(VERTEX_ATTR_TEXCOORDS, 2, GL_FLOAT, GL_FALSE, sizeof(glimac::ShapeVertex), (const GLvoid*)(offsetof(glimac::ShapeVertex, texCoords)));

    this->m_vboCharacter.unBind();
    this->m_vaoCharacter.unBind();

    this->m_cameraDirections = {
        {GL_TEXTURE_CUBE_MAP_POSITIVE_X, 180.f, -90.f},
        {GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 180.f, 90.f},
        {GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 90.f, 180.f},
        {GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, -90.f, 0.f},
        {GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 180.f, 180.f},
        {GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 180.f, 0.f}};

    this->m_shadowProgram = ShadowProgram();

    m_viewMatrixLight.setPos(this->m_scene.m_pointLight._lightPos);

    this->m_shadowProgram.m_Program.use();

    glEnable(GL_POLYGON_OFFSET_FILL);
    float factor = 1.f;
    float units  = 1.f;
    glPolygonOffset(factor, units);

    glEnable(GL_DEPTH_TEST);
}

void Renderer::render(p6::Context& ctx, ShadowCubeMapFBO& shadowMap)
{
    // ctx.render_to_main_canvas();
    this->m_shadowProgram.m_Program.use();
    std::cout << "2 : " << &shadowMap << "\n";

    this->m_scene.updateGlints(ctx, this->m_shadowProgram.m_Program.id());
    for (size_t i = 0; i < 6; i++)
    {
        shadowMap.BindForWriting(m_cameraDirections[i].CubemapFace);

        glClear(GL_DEPTH_BUFFER_BIT);

        m_viewMatrixLight.setPos(this->m_scene.m_pointLight._lightPos);
        m_viewMatrixLight.setTheta(m_cameraDirections[i].theta);
        m_viewMatrixLight.setPhi(m_cameraDirections[i].phi);

        // this->renderCamera(ctx);
        this->renderLights(ctx);

        this->m_vaoBoids.bind();
        this->renderBoidsShadows(ctx);
        this->m_vaoBoids.unBind();

        this->m_vaoCharacter.bind();
        this->renderCharacterShadows(ctx);
        this->m_vaoCharacter.unBind();

        this->m_vaoEnvironment.bind();
        this->renderEnvironmentShadows(ctx);
        this->m_vaoEnvironment.unBind();
    }

    ctx.render_to_main_canvas();

    glViewport(0, 0, ctx.current_canvas_width(), ctx.current_canvas_height());

    glClearColor(0.5f, 0.5f, 0.5f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    this->m_shader.use();
    this->m_scene.updateGlints(ctx, this->m_shader.id());

    this->renderCamera(ctx);
    this->renderLights(ctx);

    this->m_vaoCharacter.bind();
    this->renderCharacter(ctx, shadowMap);
    this->m_vaoCharacter.unBind();

    this->m_vaoEnvironment.bind();
    this->renderEnvironment(ctx, shadowMap);
    this->m_vaoEnvironment.unBind();

    this->m_vaoBoids.bind();
    this->renderBoids(ctx, shadowMap);
    this->m_vaoBoids.unBind();
}

void Renderer::renderLights(p6::Context& ctx)
{
    glm::vec3 uMVLightsPos;

    // playing a bit
    this->m_scene.m_pointLight.initialLightPos.x = glm::sin(ctx.time()) * 5;

    this->m_scene.m_pointLight._lightPos = this->m_scene.m_pointLight.initialLightPos - this->m_scene.m_character.m_position;

    uMVLightsPos = glm::vec3(this->m_scene.m_camera.getViewMatrix() * glm::vec4(this->m_scene.m_pointLight._lightPos, 1));

    glUniform3fv(this->m_scene.m_pointLight.uLightPos_vs, 1, glm::value_ptr(uMVLightsPos));
    glUniform3fv(this->m_scene.m_pointLight.uLightIntensity, 1, glm::value_ptr(this->m_scene.m_pointLight._uLightIntensity));
    glUniform3fv(this->m_scene.m_pointLight.uAmbient, 1, glm::value_ptr(this->m_scene.m_pointLight._uAmbient));

    // glUniform3fv(this->m_scene.m_dirLight.uLightDir_vs, 1, glm::value_ptr(this->m_scene.m_dirLight._lightDir));
    // glUniform3fv(this->m_scene.m_dirLight.uLightIntensity, 1, glm::value_ptr(this->m_scene.m_dirLight._uLightIntensity));
}

void Renderer::renderCamera(p6::Context& ctx)
{
    float movementStrength = 50.f;
    this->m_scene.m_camera.rotateLeft(ctx.mouse_delta().y * movementStrength);
    this->m_scene.m_camera.rotateUp(ctx.mouse_delta().x * movementStrength);
}

void Renderer::renderBoids(p6::Context& ctx, ShadowCubeMapFBO& shadowMap)
{
    glUniform3fv(this->m_scene.m_boidLightTexture.uKd, 1, glm::value_ptr(this->m_scene.m_boidLightTexture._uKd[0]));
    glUniform3fv(this->m_scene.m_boidLightTexture.uKs, 1, glm::value_ptr(this->m_scene.m_boidLightTexture._uKs[0]));
    glUniform1f(this->m_scene.m_boidLightTexture.uShininess, this->m_scene.m_boidLightTexture._uShininess[0]);

    std::cout << "3 : " << &shadowMap << "\n";

    for (size_t i = 0; i < this->m_boidsContainer.size(); i++)
    {
        shadowMap.BindForReading(GL_TEXTURE1);

        this->m_scene.m_boidTextures.bindTexture(GL_TEXTURE0);

        this->m_boidsContainer[i].moove(ctx);
        this->m_boidsContainer[i].checkBorder(ctx, this->m_scene.m_environment);
        // this->m_boidsContainer[i].avoidCharacter(ctx, this->m_scene.m_character);

        // CALCUL LA ROTATION DU BOIDS EN FONCTION DE SA DIRECTION
        glm::vec3 direction = glm::normalize(this->m_boidsContainer[i].m_direction);
        glm::vec3 up        = glm::vec3(0.0f, 0.0f, 1.0f); // boids orientés vers le haut par défaut
        glm::vec3 axis      = glm::normalize(glm::cross(up, direction));
        float     angle     = glm::radians(glm::degrees(glm::acos(glm::dot(up, direction))));

        this->m_scene.m_boidsTransformations.MVMatrix = glm::translate(this->m_scene.m_camera.getViewMatrix(), this->m_boidsContainer[i].m_position);
        this->m_scene.m_boidsTransformations.MVMatrix = glm::translate(this->m_scene.m_boidsTransformations.MVMatrix, -this->m_scene.m_character.m_position);
        this->m_scene.m_boidsTransformations.MVMatrix = glm::rotate(this->m_scene.m_boidsTransformations.MVMatrix, angle, axis);
        this->m_scene.m_boidsTransformations.MVMatrix = glm::scale(this->m_scene.m_boidsTransformations.MVMatrix, glm::vec3(this->m_boidsContainer[i].m_size, this->m_boidsContainer[i].m_size, this->m_boidsContainer[i].m_size));

        this->m_scene.m_boidsTransformations.MMatrix = glm::translate(glm::mat4(1.f), this->m_boidsContainer[i].m_position);
        this->m_scene.m_boidsTransformations.MMatrix = glm::translate(this->m_scene.m_boidsTransformations.MMatrix, -this->m_scene.m_character.m_position);
        this->m_scene.m_boidsTransformations.MMatrix = glm::rotate(this->m_scene.m_boidsTransformations.MMatrix, angle, axis);
        this->m_scene.m_boidsTransformations.MMatrix = glm::scale(this->m_scene.m_boidsTransformations.MMatrix, glm::vec3(this->m_boidsContainer[i].m_size, this->m_boidsContainer[i].m_size, this->m_boidsContainer[i].m_size));

        glUniformMatrix4fv(this->m_scene.m_boidsTransformations.uMMatrix, 1, GL_FALSE, glm::value_ptr(this->m_scene.m_boidsTransformations.MMatrix));

        glUniform1f(this->m_scene.m_boidLightTexture.ufar_plane, m_far);
        glUniform3fv(this->m_scene.m_pointLight.uLightPos, 1, glm::value_ptr(this->m_scene.m_pointLight._lightPos));
        glUniform1i(this->m_scene.m_boidLightTexture.uTexture, 1);

        glUniformMatrix4fv(this->m_scene.m_boidsTransformations.uMVPMatrix, 1, GL_FALSE, glm::value_ptr(this->m_scene.m_boidsTransformations.ProjMatrix * this->m_scene.m_boidsTransformations.MVMatrix));
        glUniformMatrix4fv(this->m_scene.m_boidsTransformations.uMVMatrix, 1, GL_FALSE, glm::value_ptr(this->m_scene.m_boidsTransformations.MVMatrix));
        glUniformMatrix4fv(this->m_scene.m_boidsTransformations.uNormalMatrix, 1, GL_FALSE, glm::value_ptr(this->m_scene.m_boidsTransformations.NormalMatrix));

        glDrawArrays(GL_TRIANGLES, 0, this->m_scene.m_boidModel.size());

        this->m_scene.m_boidTextures.unbindTexture();

        glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
    }
}

void Renderer::renderBoidsShadows(p6::Context& ctx)
{
    for (size_t i = 0; i < this->m_boidsContainer.size(); i++)
    {
        glm::vec3 direction = glm::normalize(this->m_boidsContainer[i].m_direction);
        glm::vec3 up        = glm::vec3(0.0f, 0.0f, 1.0f); // boids orientés vers le haut par défaut
        glm::vec3 axis      = glm::normalize(glm::cross(up, direction));
        float     angle     = glm::radians(glm::degrees(glm::acos(glm::dot(up, direction))));

        this->m_scene.m_boidsTransformations.MMatrix = glm::translate(glm::mat4(1.f), this->m_boidsContainer[i].m_position);
        this->m_scene.m_boidsTransformations.MMatrix = glm::translate(this->m_scene.m_boidsTransformations.MMatrix, -this->m_scene.m_character.m_position);
        this->m_scene.m_boidsTransformations.MMatrix = glm::rotate(this->m_scene.m_boidsTransformations.MMatrix, angle, axis);
        this->m_scene.m_boidsTransformations.MMatrix = glm::scale(this->m_scene.m_boidsTransformations.MMatrix, glm::vec3(this->m_boidsContainer[i].m_size, this->m_boidsContainer[i].m_size, this->m_boidsContainer[i].m_size));

        glUniform1f(m_shadowProgram.ufar_plane, m_far);
        glUniform3fv(m_shadowProgram.uLightPos, 1, glm::value_ptr(this->m_scene.m_pointLight._lightPos));
        glUniformMatrix4fv(m_shadowProgram.uMMatrix, 1, GL_FALSE, glm::value_ptr(this->m_scene.m_boidsTransformations.MMatrix));
        glUniformMatrix4fv(m_shadowProgram.uMVPLight, 1, GL_FALSE, glm::value_ptr(m_shadowProj * m_viewMatrixLight.getViewMatrix() * this->m_scene.m_boidsTransformations.MMatrix));

        glDrawArrays(GL_TRIANGLES, 0, this->m_scene.m_boidModel.size());
    }
}

void Renderer::renderEnvironment(p6::Context& ctx, ShadowCubeMapFBO& shadowMap)
{
    shadowMap.BindForReading(GL_TEXTURE1);
    this->m_scene.m_environmentTextures.bindTexture(GL_TEXTURE0);

    this->m_scene.m_environmentTransformations.MVMatrix = glm::translate(this->m_scene.m_camera.getViewMatrix(), this->m_scene.m_environment.m_position);
    this->m_scene.m_environmentTransformations.MVMatrix = glm::translate(this->m_scene.m_environmentTransformations.MVMatrix, -this->m_scene.m_character.m_position);

    // this->m_scene.m_environmentTransformations.MVMatrix = glm::rotate(this->m_scene.m_environmentTransformations.MVMatrix, glm::radians(90.f), glm::vec3(1.f, 0.f, 0.f));
    this->m_scene.m_environmentTransformations.MVMatrix = glm::scale(this->m_scene.m_environmentTransformations.MVMatrix, glm::vec3(this->m_scene.m_environment.m_sizeX, this->m_scene.m_environment.m_sizeY, this->m_scene.m_environment.m_sizeZ));

    glUniform3fv(this->m_scene.m_environmentLightTexture.uKd, 1, glm::value_ptr(this->m_scene.m_environmentLightTexture._uKd[0]));
    glUniform3fv(this->m_scene.m_environmentLightTexture.uKs, 1, glm::value_ptr(this->m_scene.m_environmentLightTexture._uKs[0]));
    glUniform1f(this->m_scene.m_environmentLightTexture.uShininess, this->m_scene.m_environmentLightTexture._uShininess[0]);

    glUniformMatrix4fv(this->m_scene.m_environmentTransformations.uMVPMatrix, 1, GL_FALSE, glm::value_ptr(this->m_scene.m_environmentTransformations.ProjMatrix * this->m_scene.m_environmentTransformations.MVMatrix));
    glUniformMatrix4fv(this->m_scene.m_environmentTransformations.uMVMatrix, 1, GL_FALSE, glm::value_ptr(this->m_scene.m_environmentTransformations.MVMatrix));

    glUniformMatrix4fv(this->m_scene.m_environmentTransformations.uMMatrix, 1, GL_FALSE, glm::value_ptr(this->m_scene.m_environmentTransformations.MMatrix));

    glUniform1f(this->m_scene.m_environmentLightTexture.ufar_plane, m_far);
    glUniform3fv(this->m_scene.m_pointLight.uLightPos, 1, glm::value_ptr(this->m_scene.m_pointLight._lightPos));
    glUniform1i(this->m_scene.m_environmentLightTexture.uTexture, 1);

    glUniform1i(this->m_scene.m_environmentLightTexture.uTexture, 1);

    glDrawArrays(GL_TRIANGLES, 0, this->m_scene.m_environmentModel.size());

    this->m_scene.m_environmentTextures.unbindTexture();

    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

void Renderer::renderEnvironmentShadows(p6::Context& ctx)
{
    this->m_scene.m_environmentTransformations.MMatrix = glm::translate(glm::mat4(1.f), this->m_scene.m_environment.m_position);
    this->m_scene.m_environmentTransformations.MMatrix = glm::translate(this->m_scene.m_environmentTransformations.MMatrix, -this->m_scene.m_character.m_position);
    this->m_scene.m_environmentTransformations.MMatrix = glm::scale(this->m_scene.m_environmentTransformations.MMatrix, glm::vec3(this->m_scene.m_environment.m_sizeX, this->m_scene.m_environment.m_sizeY, this->m_scene.m_environment.m_sizeZ));

    glUniform1f(m_shadowProgram.ufar_plane, m_far);
    glUniform3fv(m_shadowProgram.uLightPos, 1, glm::value_ptr(this->m_scene.m_pointLight._lightPos));
    glUniformMatrix4fv(m_shadowProgram.uMMatrix, 1, GL_FALSE, glm::value_ptr(this->m_scene.m_environmentTransformations.MMatrix));
    glUniformMatrix4fv(m_shadowProgram.uMVPLight, 1, GL_FALSE, glm::value_ptr(m_shadowProj * m_viewMatrixLight.getViewMatrix() * this->m_scene.m_environmentTransformations.MMatrix));

    glDrawArrays(GL_TRIANGLES, 0, this->m_scene.m_environmentModel.size());
}

void Renderer::renderCharacter(p6::Context& ctx, ShadowCubeMapFBO& shadowMap)
{
    shadowMap.BindForReading(GL_TEXTURE1);

    float angleY = glm::radians(-this->m_scene.m_camera.getAngleY());

    this->m_scene.m_character.m_angleY = angleY;

    this->m_scene.m_characterTransformations.MVMatrix = glm::translate(this->m_scene.m_camera.getViewMatrix(), glm::vec3(0, -1, 0));
    this->m_scene.m_characterTransformations.MVMatrix = glm::rotate(this->m_scene.m_characterTransformations.MVMatrix, angleY, glm::vec3(0.f, 1.f, 0.f));
    this->m_scene.m_characterTransformations.MVMatrix = glm::scale(this->m_scene.m_characterTransformations.MVMatrix, glm::vec3(this->m_scene.m_character.m_sizeX, this->m_scene.m_character.m_sizeY, this->m_scene.m_character.m_sizeZ));

    glUniform3fv(this->m_scene.m_characterLightTexture.uKd, 1, glm::value_ptr(this->m_scene.m_characterLightTexture._uKd[0]));
    glUniform3fv(this->m_scene.m_characterLightTexture.uKs, 1, glm::value_ptr(this->m_scene.m_characterLightTexture._uKs[0]));
    glUniform1f(this->m_scene.m_characterLightTexture.uShininess, this->m_scene.m_characterLightTexture._uShininess[0]);

    this->m_scene.m_characterTransformations.MMatrix = glm::translate(glm::mat4(1.f), glm::vec3(0, -1, 0));
    this->m_scene.m_characterTransformations.MMatrix = glm::rotate(this->m_scene.m_characterTransformations.MMatrix, angleY, glm::vec3(0.f, 1.f, 0.f));
    this->m_scene.m_characterTransformations.MMatrix = glm::scale(this->m_scene.m_characterTransformations.MMatrix, glm::vec3(this->m_scene.m_character.m_sizeX, this->m_scene.m_character.m_sizeY, this->m_scene.m_character.m_sizeZ));

    glUniformMatrix4fv(this->m_scene.m_characterTransformations.uMMatrix, 1, GL_FALSE, glm::value_ptr(this->m_scene.m_characterTransformations.MMatrix));

    glUniformMatrix4fv(this->m_scene.m_characterTransformations.uMVPMatrix, 1, GL_FALSE, glm::value_ptr(this->m_scene.m_characterTransformations.ProjMatrix * this->m_scene.m_characterTransformations.MVMatrix));
    glUniformMatrix4fv(this->m_scene.m_characterTransformations.uMVMatrix, 1, GL_FALSE, glm::value_ptr(this->m_scene.m_characterTransformations.MVMatrix));
    glUniformMatrix4fv(this->m_scene.m_characterTransformations.uNormalMatrix, 1, GL_FALSE, glm::value_ptr(this->m_scene.m_characterTransformations.NormalMatrix));

    glUniform1f(this->m_scene.m_characterLightTexture.ufar_plane, m_far);
    glUniform3fv(this->m_scene.m_pointLight.uLightPos, 1, glm::value_ptr(this->m_scene.m_pointLight._lightPos));
    glUniform1i(this->m_scene.m_characterLightTexture.uTexture, 1);

    glDrawArrays(GL_TRIANGLES, 0, this->m_scene.m_characterModel.size());

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

void Renderer::renderCharacterShadows(p6::Context& ctx)
{
    float angleY = glm::radians(-this->m_scene.m_camera.getAngleY());

    this->m_scene.m_character.m_angleY = angleY;

    this->m_scene.m_characterTransformations.MMatrix = glm::translate(glm::mat4(1.f), this->m_scene.m_character.m_position);

    this->m_scene.m_characterTransformations.MMatrix = glm::rotate(this->m_scene.m_characterTransformations.MMatrix, angleY, glm::vec3(0.f, 1.f, 0.f));
    this->m_scene.m_characterTransformations.MMatrix = glm::scale(this->m_scene.m_characterTransformations.MMatrix, glm::vec3(this->m_scene.m_character.m_sizeX, this->m_scene.m_character.m_sizeY, this->m_scene.m_character.m_sizeZ));

    glUniform1f(m_shadowProgram.ufar_plane, m_far);
    glUniform3fv(m_shadowProgram.uLightPos, 1, glm::value_ptr(this->m_scene.m_pointLight._lightPos));
    glUniformMatrix4fv(m_shadowProgram.uMMatrix, 1, GL_FALSE, glm::value_ptr(this->m_scene.m_characterTransformations.MMatrix));
    glUniformMatrix4fv(m_shadowProgram.uMVPLight, 1, GL_FALSE, glm::value_ptr(m_shadowProj * m_viewMatrixLight.getViewMatrix() * this->m_scene.m_characterTransformations.MMatrix));

    glDrawArrays(GL_TRIANGLES, 0, this->m_scene.m_characterModel.size());
}

void Renderer::deleteBuffers()
{
    this->m_vboBoids.deleteVbo();
    this->m_vboEnvironment.deleteVbo();
    this->m_vboCharacter.deleteVbo();

    this->m_vaoBoids.deleteVao();
    this->m_vaoEnvironment.deleteVao();
    this->m_vaoCharacter.deleteVao();
}

void Renderer::updateBoidVbo()
{
    this->m_vboBoids.bind();
    glBufferData(GL_ARRAY_BUFFER, this->m_scene.m_boidModel.size() * sizeof(glimac::ShapeVertex), this->m_scene.m_boidModel.data(), GL_STATIC_DRAW);
    this->m_vboBoids.unBind();
}

void Renderer::updateEnvironmentVbo()
{
    this->m_vboEnvironment.bind();
    glBufferData(GL_ARRAY_BUFFER, this->m_scene.m_environmentModel.size() * sizeof(glimac::ShapeVertex), this->m_scene.m_environmentModel.data(), GL_STATIC_DRAW);
    this->m_vboEnvironment.unBind();
}