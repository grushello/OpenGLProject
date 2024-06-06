#ifndef GAMELEVEL_H
#define GAMELEVEL_H


#include "internal/Tile.h"
#include "internal/Entity.h"
#include "internal/SpriteRenderer.h"
#include "internal/Player.h"
#include "internal/Box.h"
#include <internal/TextRenderer.h>

#include <vector>
class GameLevel
{
public:
    std::vector<Tile*> tiles;
    std::vector<Tile*> goalTiles;
    std::vector<Entity*> entities;

    Player* player;

    GameLevel() : player(nullptr){ }
    ~GameLevel() { }

    void Load(const char* file, unsigned int levelWidth, unsigned int levelHeight);
    void Draw(SpriteRenderer& renderer);
    bool IsCompleted();
    void ProcessGameLogic(PlayerActions action);

private:

    void init(std::vector<std::vector<unsigned int>>& tileData, std::vector<std::vector<unsigned int>>& entityData, unsigned int levelWidth, unsigned int levelHeight);
    void tileInit(int tile_ID, float unit_width, float unit_height, unsigned int tile_x, unsigned int tile_y);
    void entityInit(int entity_ID, float unit_width, float unit_height, unsigned int entity_x, unsigned int entity_y);

};

#endif