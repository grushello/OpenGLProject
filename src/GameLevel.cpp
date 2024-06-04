#include "internal/GameLevel.h"
#include "internal/GameLogic.h"
#include <fstream>
#include <sstream>
#include <iostream>

void GameLevel::Load(const char* file, unsigned int levelWidth, unsigned int levelHeight)
{
    this->tiles.clear();
    this->entities.clear();
    this->player = nullptr;

    unsigned int tileCode, entityCode;
    std::string line;
    std::ifstream fstream(file);
    std::vector<std::vector<unsigned int>> tileData;
    std::vector<std::vector<unsigned int>> entityData;
    if (fstream)
    {
        while (std::getline(fstream, line) && line != "/") // read each line from level file (tiles)
        {
            std::istringstream sstream(line);
            std::vector<unsigned int> row;
            while (sstream >> tileCode) // read each word separated by spaces
                row.push_back(tileCode);
            tileData.push_back(row);
        }
        while (std::getline(fstream, line)) // read each line from level file (entities)
        {
            std::istringstream sstream(line);
            std::vector<unsigned int> row;
            while (sstream >> entityCode) // read each word separated by spaces
                row.push_back(entityCode);
            entityData.push_back(row);
        }
        if (tileData.size() > 0)
            this->init(tileData, entityData, levelWidth, levelHeight);
    }
}

void GameLevel::Draw(SpriteRenderer& renderer)
{
    for (Tile* tile : this->tiles)
        tile->draw(renderer);
    for (Entity* entity : this->entities)
        entity->draw(renderer);
}

bool GameLevel::IsCompleted()
{
    return GameLogic::IsCompleted(this);
}

void GameLevel::init(std::vector<std::vector<unsigned int>>& tileData, std::vector<std::vector<unsigned int>> &entityData, unsigned int levelWidth, unsigned int levelHeight)
{
    unsigned int height = tileData.size();
    unsigned int width = tileData[0].size();
    float unit_width = TILE_WIDTH, unit_height = TILE_HEIGHT;

    for (unsigned int y = 0; y < height; ++y)
    {
        for (unsigned int x = 0; x < width; ++x)
        {
            tileInit(tileData[y][x], unit_width, unit_height, x * unit_width, y * unit_height);
            entityInit(entityData[y][x], unit_width, unit_height, x * unit_width, y * unit_height);
        }
    }
}

void GameLevel::tileInit(int tile_ID, float unit_width, float unit_height, unsigned int tile_x, unsigned int tile_y)
{
    if (tile_ID == TILE_NONE)
        return;

    glm::vec2 pos(tile_x, tile_y);
    glm::vec2 size(unit_width, unit_height);
    glm::vec3 color(1.0f, 1.0f, 1.0f);

    Tile* tile = new Tile(pos, size, tile_ID, color);
    this->tiles.push_back(tile);
    if (tile->ID == TILE_GOAL)
    {
        goalTiles.push_back(tile);
    }
}

void GameLevel::entityInit(int entity_ID, float unit_width, float unit_height, unsigned int entity_x, unsigned int entity_y)
{
    if (entity_ID == ENTITY_NONE)
        return;

    glm::vec2 pos(entity_x, entity_y);
    glm::vec2 size(unit_width, unit_height);
    glm::vec3 color(1.0f, 1.0f, 1.0f);

    Player* player = nullptr;
    Box* box = nullptr;

    switch (entity_ID)
    {
    case ENTITY_PLAYER:
        player = new Player(pos, size, color);
        this->player = player;
        this->entities.push_back(player);
        break;
    case ENTITY_BOX:
        box = new Box(pos, size, color);
        entities.push_back(box);
        break;
    case ENTITY_ENEMY:
        return;
    default:
        std::cerr << "incorrect entity ID was passed";
        break;
    }
}

void GameLevel::ProcessGameLogic(PlayerActions action)
{
    GameLogic::ProcessGameLogic(action, this);
}