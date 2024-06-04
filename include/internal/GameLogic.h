#ifndef GAMELOGIC_H
#define GAMELOGIC_H

#include<internal/Player.h>
#include<internal/tile.h>
#include<internal/GameLevel.h>

class GameLogic
{
public:
	static void ProcessGameLogic(PlayerActions action, GameLevel* gameLevel);
	static bool IsCompleted(GameLevel* gameLevel);
private:
	static bool MoveBox(Box* box, glm::vec2 direction, GameLevel* gameLevel);
	static void FillHole(Box* box, Tile* hole, GameLevel* gameLevel);
	static void FillBigHole(Box* box, Tile* bigHole, GameLevel* gameLevel);
	static Entity* GetEntityAtPosition(glm::vec2 position, GameLevel* gameLevel);
	static Tile* GetTileAtPosition(glm::vec2 position, GameLevel* gameLevel);
	static bool IsValidMove(glm::vec2 position, GameLevel* gameLevel);
};

#endif
