#include "internal/Box.h"

Box::Box(glm::vec2 pos, glm::vec2 size, glm::vec3 color)
    : Entity(pos, size, color)
{
    this->loadTexture();
}

void Box::loadTexture()
{
    this->Sprite = ResourceManager::GetTexture("entity_box");
}