#include "internal/GameLevel.h"

#include <fstream>
#include <sstream>


void GameLevel::Load(const char *file, unsigned int levelWidth, unsigned int levelHeight)
{
    // clear old data
    this->tiles.clear();
    // load from file
    unsigned int tileCode, entityCode;
    GameLevel level;
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

void GameLevel::Draw(SpriteRenderer &renderer)
{
    for (Tile &tile : this->tiles)
        tile.Draw(renderer);
    for (Entity& entity : this->entities)
        entity.Draw(renderer);
}

bool GameLevel::IsCompleted()
{
    /*if (player.tilepos == finishTilePos)
        return true;*/
    return false;
}

void GameLevel::init(std::vector<std::vector<unsigned int>> tileData, std::vector<std::vector<unsigned int>> entityData, unsigned int levelWidth, unsigned int levelHeight)
{
    // calculate dimensions
    unsigned int height = tileData.size();
    unsigned int width = tileData[0].size(); // note we can index vector at [0] since this function is only called if height > 0
    float unit_width = TILE_WIDTH, unit_height = TILE_HEIGHT; 
    // initialize level tiles based on tileData		
    for (unsigned int y = 0; y < height; ++y)
    {
        for (unsigned int x = 0; x < width; ++x)
        {
            // check block type from level data (2D level array)
            tileInit(tileData[y][x], unit_width, unit_height, x, y);
            entityInit(entityData[y][x], unit_width, unit_height, x, y);
        }
    }
}
void GameLevel::tileInit(int tile_ID, float unit_width, float unit_height, unsigned int tile_x, unsigned int tile_y)
{
    switch (tile_ID)
    {
    case TILE_NONE:
        return;
    case TILE_FINISH:
        finishTilePos = glm::vec2(tile_x, tile_y);
    default:
        break;
    }

    glm::vec2 pos(unit_width * tile_x, unit_height * tile_y);
    glm::vec2 size(unit_width, unit_height);
    glm::vec3 color(1.0f, 1.0f, 1.0f);

    Tile tile(pos, size, tile_ID, color);
    this->tiles.push_back(tile);
}
void GameLevel::entityInit(int entity_ID, float unit_width, float unit_height, unsigned int entity_x, unsigned int entity_y)
{
    if (entity_ID == ENTITY_NONE)
        return;

    glm::vec2 pos(unit_width * entity_x, unit_height * entity_y);
    glm::vec2 size(unit_width, unit_height);
    glm::vec3 color(1.0f, 1.0f, 1.0f);

    Entity entity(pos, size, entity_ID, color);
    this->entities.push_back(entity);
}