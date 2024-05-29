#include "internal/Entity.h"

Entity::Entity(glm::vec2 pos, glm::vec2 size, glm::vec3 color)
    : Position(pos), Size(size), Color(color), Rotation(0.0f) { }

void Entity::draw(SpriteRenderer& renderer)
{
    renderer.drawSprite(this->Sprite, this->Position, this->Size, this->Rotation, this->Color);
}