#ifndef GAME_H
#define GAME_H

#include "internal/Resource_Manager.h"
#include "internal/GameLevel.h"
#include "internal/GameData.h"
#include <vector>
class Game
{
private:
public:
    SpriteRenderer* Renderer;
    GameData& gameData = GameData::getInstance();
    bool Keys[1024];
    bool KeysProcessed[1024];
    unsigned int Width, Height;
    std::vector<GameLevel> Levels;

    Game(unsigned int width, unsigned int height);
    ~Game();

    void Load();
    void ProcessInput(float dt);
    void Update(float dt);
    void Render();
    void ResetLevel();
};

#endif
