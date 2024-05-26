#include "internal/Entity.h"

Entity::Entity()
    : Position(0.0f, 0.0f), Size(1.0f, 1.0f), Color(1.0f), Rotation(0.0f), Sprite() { }

Entity::Entity(glm::vec2 pos, glm::vec2 size, int entity_ID, glm::vec3 color)
    : Position(pos), Size(size), Color(color), Rotation(0.0f)
{
    switch (entity_ID)
    {
    case ENTITY_NONE:
        std::cerr << "Entity ID can't be equal to ENTITY_NONE\n";
        return;
    case ENTITY_PLAYER:
        this->Sprite = ResourceManager::GetTexture("entity_player");
        break;
    case ENTITY_BOX:
        this->Sprite = ResourceManager::GetTexture("entity_box");
        break;
    case ENTITY_ENEMY:
        this->Sprite = ResourceManager::GetTexture("entity_enemy");
        break;
    default:
        break;
    }
}

void Entity::draw(SpriteRenderer& renderer)
{
    renderer.drawSprite(this->Sprite, this->Position, this->Size, this->Rotation, this->Color);
}