#include "internal/Player.h"

Player::Player(glm::vec2 pos, glm::vec2 size, glm::vec3 color)
    : Entity(pos, size, color)
{
    this->loadTexture();
}


void Player::loadTexture()
{
    this->Sprite = ResourceManager::GetTexture("entity_player");
}