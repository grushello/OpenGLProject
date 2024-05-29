#ifndef ENTITY_H
#define ENTITY_H

#include "internal/SpriteRenderer.h"
#include "internal/Resource_manager.h"

#include <iostream>

const float ENTITY_WIDTH = 64;
const float ENTITY_HEIGHT = 64;

enum EntityID
{
    ENTITY_NONE,
    ENTITY_BOX,
    ENTITY_PLAYER,
    ENTITY_ENEMY
};
class Entity
{
public:
    glm::vec2   Position, Size;
    glm::vec3   Color;
    float       Rotation;
    Texture2D   Sprite;

    Entity(glm::vec2 pos = glm::vec2(0.0f, 0.0f), glm::vec2 size = glm::vec2(ENTITY_WIDTH, ENTITY_HEIGHT), glm::vec3 color = glm::vec3(1.0f));
    virtual ~Entity() = default;

    virtual void draw(SpriteRenderer& renderer);
    virtual void loadTexture() = 0;
};

#endif
