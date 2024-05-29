#ifndef SPRITE_RENDERER_H
#define SPRITE_RENDERER_H

#include "texture.h"
#include "shader.h"
#include "Camera.h"


class SpriteRenderer
{
public:
    // Constructor (inits shaders/shapes)
    SpriteRenderer(Shader& shader, Camera2D& camera);
    // Destructor
    ~SpriteRenderer();
    // Renders a defined quad textured with given sprite
    void drawSprite(Texture2D& texture, glm::vec2 position, glm::vec2 size = glm::vec2(10.0f, 10.0f), float rotate = 0.0f, glm::vec3 color = glm::vec3(1.0f));
    void drawBackground(Texture2D& backgroundTexture, glm::vec2 screenSize);
    // Render state
    Shader       shader;
    Camera2D     camera;
    unsigned int quadVAO;
    // Initializes and configures the quad's buffer and vertex attributes
    void initRenderData();
};

#endif
