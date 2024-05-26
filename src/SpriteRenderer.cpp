#include "internal/SpriteRenderer.h"
#include "iostream"

SpriteRenderer::SpriteRenderer(Shader& shader, Camera2D& camera)
{
    this->shader = shader;
    this->camera = camera;
    this->initRenderData();
}

SpriteRenderer::~SpriteRenderer()
{
    glDeleteVertexArrays(1, &this->quadVAO);
}

void SpriteRenderer::drawSprite(Texture2D& texture, glm::vec2 position, glm::vec2 size, float rotate, glm::vec3 color)
{
    // prepare transformations
    this->shader.Use();
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(position, 0.0f));  // first translate (transformations are: scale happens first, then rotation, and then final translation happens; reversed order)
    model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.0f)); // move origin of rotation to center of quad
    model = glm::rotate(model, glm::radians(rotate), glm::vec3(0.0f, 0.0f, 1.0f)); // then rotate
    model = glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.0f)); // move origin back
    model = glm::scale(model, glm::vec3(size, 1.0f)); // last scale

    glm::mat4 view = camera.getViewMatrix();
    glm::mat4 projection = camera.getProjectionMatrix();

    this->shader.SetMatrix4("view", view);
    this->shader.SetMatrix4("model", model);
    this->shader.SetMatrix4("projection", projection);

    // render textured quad
    this->shader.SetVector3f("spriteColor", color);

    glActiveTexture(GL_TEXTURE0);
    texture.Bind();

    glBindVertexArray(this->quadVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}
void SpriteRenderer::drawBackground(Texture2D& backgroundTexture, glm::vec2 screenSize)
{
    // prepare transformations
    this->shader.Use();
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0, 0, 0.0f)); // place background at the center of the screen
    model = glm::scale(model, glm::vec3(screenSize.x, screenSize.y, 1.0f)); // scale to cover the entire screen

    glm::mat4 view = glm::mat4(1.0f); // identity matrix for view
    glm::mat4 projection = glm::ortho(0.0f, screenSize.x, screenSize.y, 0.0f, -1.0f, 1.0f); // orthographic projection for 2D rendering

    this->shader.SetMatrix4("view", view);
    this->shader.SetMatrix4("model", model);
    this->shader.SetMatrix4("projection", projection);

    // render background quad
    this->shader.SetVector3f("spriteColor", glm::vec3(1.0f)); // background should not be affected by color

    glActiveTexture(GL_TEXTURE0);
    backgroundTexture.Bind();

    glBindVertexArray(this->quadVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}
void SpriteRenderer::initRenderData()
{
    // configure VAO/VBO
    unsigned int VBO;
    float vertices[] = {
        // pos      // tex
        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 0.0f,

        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f
    };

    glGenVertexArrays(1, &this->quadVAO);
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindVertexArray(this->quadVAO);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}
