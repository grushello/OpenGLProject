#include "internal/GameLogic.h"
#include "internal/GameData.h"
#include <vector>

bool GameLogic::IsCompleted(GameLevel* gameLevel)
{
    for (Tile* tile : gameLevel->goalTiles)
    {
        Entity* entity = GetEntityAtPosition(tile->Position, gameLevel);
        Box* box = dynamic_cast<Box*>(entity);
        if (!box)
        {
            return false;
        }
    }
    return true;
}

void GameLogic::ProcessGameLogic(PlayerActions action, GameLevel* gameLevel)
{
    GameData& gameData = GameData::getInstance();
    if (!gameLevel->player)
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

    glm::vec2 newPosition = gameLevel->player->Position + direction;

    if (IsValidMove(newPosition, gameLevel))
    {
        gameLevel->player->Position = newPosition;
    }
    else
    {
        Entity* entity = GetEntityAtPosition(newPosition, gameLevel);
        if (Box* box = dynamic_cast<Box*>(entity))
        {
            if (MoveBox(box, direction, gameLevel))
            {
                gameLevel->player->Position = newPosition;
                gameData.setBoxesMoved(gameData.getBoxesMoved() + 1);
            }
        }
    }
    if (newPosition == gameLevel->player->Position)
    {
        gameData.setTilesTraveled(gameData.getTilesTraveled() + 1);
    }
}

bool GameLogic::IsValidMove(glm::vec2 position, GameLevel* gameLevel)
{
    Tile* tile = GetTileAtPosition(position, gameLevel);
    Entity* entity = GetEntityAtPosition(position, gameLevel);

    return (tile && tile->IsWalkable && !entity);
}

bool GameLogic::MoveBox(Box* box, glm::vec2 direction, GameLevel* gameLevel)
{
    glm::vec2 newBoxPosition = box->Position + direction;
    Tile* tile = GetTileAtPosition(newBoxPosition, gameLevel);
    if (tile->ID == TILE_HOLE)
    {
        FillHole(box, tile, gameLevel);
        return true;
    }
    else if (tile->ID == TILE_BIG_HOLE)
    {
        FillBigHole(box, tile, gameLevel);
        return true;
    }
    if (IsValidMove(newBoxPosition, gameLevel) && !GetEntityAtPosition(newBoxPosition, gameLevel))
    {
        box->Position = newBoxPosition;
        return true;
    }
    return false;
}

void GameLogic::FillHole(Box* box, Tile* hole, GameLevel* gameLevel)
{
    GameData& gameData = GameData::getInstance();
    auto itBox = std::find(gameLevel->entities.begin(), gameLevel->entities.end(), box);
    if (itBox != gameLevel->entities.end())
    {
        gameLevel->entities.erase(itBox);
    }
    delete box;

    auto itHole = std::find(gameLevel->tiles.begin(), gameLevel->tiles.end(), hole);
    if (itHole != gameLevel->tiles.end())
    {
        *itHole = new Tile(hole->Position, glm::vec2(TILE_WIDTH, TILE_HEIGHT), TILE_FILLED_HOLE);
    }
    gameData.setHolesFilled(gameData.getHolesFilled() + 1);
}

void GameLogic::FillBigHole(Box* box, Tile* bigHole, GameLevel* gameLevel)
{
    auto itBox = std::find(gameLevel->entities.begin(), gameLevel->entities.end(), box);
    if (itBox != gameLevel->entities.end())
    {
        gameLevel->entities.erase(itBox);
    }
    delete box;

    auto itHole = std::find(gameLevel->tiles.begin(), gameLevel->tiles.end(), bigHole);
    if (itHole != gameLevel->tiles.end())
    {
        *itHole = new Tile(bigHole->Position, glm::vec2(TILE_WIDTH, TILE_HEIGHT), TILE_HOLE);
    }
}

Entity* GameLogic::GetEntityAtPosition(glm::vec2 position, GameLevel* gameLevel)
{
    for (Entity* entity : gameLevel->entities)
    {
        if (entity->Position == position)
        {
            return entity;
        }
    }
    return nullptr;
}

Tile* GameLogic::GetTileAtPosition(glm::vec2 position, GameLevel* gameLevel)
{
    for (Tile* tile : gameLevel->tiles)
    {
        if (tile->Position == position)
        {
            return tile;
        }
    }
    return nullptr;
}