#include "internal/GameLevel.h"
#include <fstream>
#include <sstream>
#include <iostream>

void GameLevel::Load(const char* file, unsigned int levelWidth, unsigned int levelHeight)
{
    // clear old data
    this->tiles.clear();
    this->entities.clear();
    this->player = nullptr;

    // load from file
    unsigned int tileCode, entityCode;
    std::string line;
    std::ifstream fstream(file);
    std::vector<std::vector<unsigned int>> tileData;
    std::vector<std::vector<unsigned int>> entityData;
    if (fstream)
    {
        while (std::getline(fstream, line) && line != "/") // read each line from level file
        {
            std::istringstream sstream(line);
            std::vector<unsigned int> row;
            while (sstream >> tileCode) // read each word separated by spaces
                row.push_back(tileCode);
            tileData.push_back(row);
        }
        while (std::getline(fstream, line)) // read each line from level file
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
    for (Tile* tile : this->goalTiles)
    {
        Entity* entity = GetEntityAtPosition(tile->Position);
        Box* box = dynamic_cast<Box*>(entity);
        if (!box)
        {
            return false;
        }
    }
    return true;
}

void GameLevel::init(std::vector<std::vector<unsigned int>> tileData, std::vector<std::vector<unsigned int>> entityData, unsigned int levelWidth, unsigned int levelHeight)
{
    unsigned int height = tileData.size();
    unsigned int width = tileData[0].size();
    float unit_width = TILE_WIDTH, unit_height = TILE_HEIGHT;

    for (unsigned int y = 0; y < height; ++y)
    {
        for (unsigned int x = 0; x < width; ++x)
        {
            tileInit(tileData[y][x], unit_width, unit_height, x, y);
            entityInit(entityData[y][x], unit_width, unit_height, x, y);
        }
    }
}

void GameLevel::tileInit(int tile_ID, float unit_width, float unit_height, unsigned int tile_x, unsigned int tile_y)
{
    if (tile_ID == TILE_NONE)
        return;

    glm::vec2 pos(unit_width * tile_x, unit_height * tile_y);
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

    glm::vec2 pos(unit_width * entity_x, unit_height * entity_y);
    glm::vec2 size(unit_width, unit_height);
    glm::vec3 color(1.0f, 1.0f, 1.0f);

    Player* player = nullptr; // Инициализируем указатели значениями по умолчанию
    Box* box = nullptr;

    switch (entity_ID)
    {
    case ENTITY_PLAYER:
        player = new Player(pos, size, color); // Используем уже объявленную переменную player
        this->player = player;
        this->entities.push_back(player);
        break;
    case ENTITY_BOX:
        box = new Box(pos, size, color); // Используем уже объявленную переменную box
        entities.push_back(box);
        break;
    case ENTITY_ENEMY:
        return;
    default:
        std::cerr << "incorrect entity ID was passed";
        break;
    }
}

void GameLevel::ProcessPlayerMovement(PlayerActions action)
{
    if (!this->player)
        return;

    glm::vec2 direction;
    switch (action)
    {
    case PLAYER_UP:
        direction = glm::vec2(0.0f, -TILE_HEIGHT);
        break;
    case PLAYER_DOWN:
        direction = glm::vec2(0.0f, TILE_HEIGHT);
        break;
    case PLAYER_LEFT:
        direction = glm::vec2(-TILE_WIDTH, 0.0f);
        break;
    case PLAYER_RIGHT:
        direction = glm::vec2(TILE_WIDTH, 0.0f);
        break;
    }

    glm::vec2 newPosition = this->player->Position + direction;

    if (IsValidPlayerMove(newPosition))
    {
        this->player->Position = newPosition;
    }
    else
    {
        Entity* entity = GetEntityAtPosition(newPosition);
        if (Box* box = dynamic_cast<Box*>(entity))
        {
            if (MoveBox(box, direction))
            {
                this->player->Position = newPosition;
            }
        }
    }
}

bool GameLevel::IsValidPlayerMove(glm::vec2 position)
{
    Tile* tile = GetTileAtPosition(position);
    Entity* entity = GetEntityAtPosition(position);

    return (tile && tile->IsWalkable && !entity);
}

bool GameLevel::IsValidBoxMove(glm::vec2 position)
{
    Tile* tile = GetTileAtPosition(position);

    return (tile && (tile->ID == TILE_HOLE || tile->IsWalkable));
}

Entity* GameLevel::GetEntityAtPosition(glm::vec2 position)
{
    for (Entity* entity : this->entities)
    {
        if (entity->Position == position)
        {
            return entity;
        }
    }
    return nullptr;
}

Tile* GameLevel::GetTileAtPosition(glm::vec2 position)
{
    for (Tile* tile : this->tiles)
    {
        if (tile->Position == position)
        {
            return tile;
        }
    }
    return nullptr;
}

bool GameLevel::MoveBox(Box* box, glm::vec2 direction)
{
    glm::vec2 newBoxPosition = box->Position + direction;
    Tile* tile = GetTileAtPosition(newBoxPosition);
    if (tile->ID == TILE_HOLE)
    {
        FillHole(box, tile);
        return true; // Возвращаем true, так как ящик был успешно перемещен на заполненное отверстие
    }
    if (IsValidBoxMove(newBoxPosition) && !GetEntityAtPosition(newBoxPosition))
    {
        box->Position = newBoxPosition;
        return true; // Возвращаем true, так как ящик был успешно перемещен
    }
    return false; // Возвращаем false, если ящик не может быть перемещен в заданном направлении
}

void GameLevel::FillHole(Box* box, Tile* hole)
{
    // Удаляем ящик из списка сущностей
    auto itBox = std::find(entities.begin(), entities.end(), box);
    if (itBox != entities.end())
    {
        entities.erase(itBox);
    }

    // Удаляем ящик из памяти
    delete box;

    // Находим индекс ямы в списке тайлов
    auto itHole = std::find(tiles.begin(), tiles.end(), hole);
    if (itHole != tiles.end())
    {
        // Заменяем текущий тайл ямы на заполненный тайл
        *itHole = new Tile(hole->Position, glm::vec2(TILE_WIDTH, TILE_HEIGHT), TILE_FILLED_HOLE);
    }
}