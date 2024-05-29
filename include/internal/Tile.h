#ifndef TILE_H
#define TILE_H

#include <internal/SpriteRenderer.h>
#include "internal/Texture.h"
#include <internal/Resource_manager.h>

#include <iostream>

const float TILE_WIDTH = 64;
const float TILE_HEIGHT = 64;
// Container object for holding all state relevant for a single
// game object entity. Each object in the game likely needs the
// minimal of state as described within GameObject.
enum TileID
{
    TILE_NONE = 0,
    TILE_GRASS = 1, // grass has 3 variations, check file names when setting the texture
    TILE_WALL = 2,
    TILE_HOLE = 3,
    TILE_FILLED_HOLE = 4,
    TILE_GOAL = 5
};
class Tile
{
public:
    int ID;
    // object state
    glm::vec2   Position, Size;
    glm::vec3   Color;
    float       Rotation;
    bool        IsWalkable;
    // render state
    Texture2D   Sprite;
    // constructor(s)
    Tile();
    Tile(glm::vec2 pos, glm::vec2 size, int ID = 0, glm::vec3 color = glm::vec3(1.0f));
    // draw sprite
    virtual void draw(SpriteRenderer& renderer);
};

#endif

