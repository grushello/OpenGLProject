#include "internal/Tile.h"


Tile::Tile()
    : Position(0.0f, 0.0f), Size(1.0f, 1.0f), Color(1.0f), Rotation(0.0f), Sprite(), IsWalkable(false) { }

Tile::Tile(glm::vec2 pos, glm::vec2 size, int ID, glm::vec3 color)
    : Position(pos), Size(size), Color(color), Rotation(0.0f), IsWalkable(false)
{
    this->Sprite = ResourceManager::GetTexture("default_texture");
    this->ID = ID;
    switch (ID)
    {
    case TILE_NONE:
        std::cerr << "Tile ID can't be equal to TILE_NONE\n";
        return;
    case TILE_GRASS:
        this->Sprite = ResourceManager::GetTexture("tile_grass"); 
        this->IsWalkable = true;
        break;
    case TILE_WALL:
        this->Sprite = ResourceManager::GetTexture("tile_wall");
        this->IsWalkable = false;
        break;
    case TILE_HOLE:
        this->Sprite = ResourceManager::GetTexture("tile_hole");
        this->IsWalkable = false;
        break;
    case TILE_FILLED_HOLE:
        this->Sprite = ResourceManager::GetTexture("tile_filled_hole");
        this->IsWalkable = true;
        break;
    case TILE_BIG_HOLE:
        this->Sprite = ResourceManager::GetTexture("tile_hole");
        this->IsWalkable = false;
        break;
    case TILE_HALF_FILLED_HOLE:
        this->Sprite = ResourceManager::GetTexture("tile_hole");
        this->IsWalkable = false;
        break;
    case TILE_GOAL:
        this->Sprite = ResourceManager::GetTexture("tile_goal");
        this->IsWalkable = true;
    default:
        break;
    }
}

void Tile::draw(SpriteRenderer& renderer)
{
    renderer.drawSprite(this->Sprite, this->Position, this->Size, this->Rotation, this->Color);
}

