#ifndef GAMELEVEL_H
#define GAMELEVEL_H

#include<vector>
#include<internal/Tile.h>
#include<internal/Entity.h>
#include<internal/Resource_manager.h>
class GameLevel
{
public:
    // level state
    std::vector<Tile> tiles;
    std::vector<Entity> entities;
    glm::vec2 finishTilePos;
    // constructor
    GameLevel() { }
    // loads level from file
    void Load(const char* file, unsigned int levelWidth, unsigned int levelHeight);
    // render level
    void Draw(SpriteRenderer& renderer);
    // check if the level is completed (all non-solid tiles are destroyed)
    bool IsCompleted();
private:
    // initialize level from tile data
    void init(std::vector<std::vector<unsigned int>> tileData,
        std::vector<std::vector<unsigned int>> entityData,
        unsigned int levelWidth, unsigned int levelHeight);
    void tileInit(int tile_ID, float unit_width, float unit_height, unsigned int tile_x, unsigned int tile_y);
    void entityInit(int entity_ID, float unit_width, float unit_height, unsigned int entity_x, unsigned int entity_y);
};
#endif
