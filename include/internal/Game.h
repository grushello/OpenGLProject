#ifndef GAME_H
#define GAME_H

#include "internal/Resource_Manager.h"
#include "internal/GameLevel.h"

#include <vector>
class Game
{
public:
    enum GameState {
        GAME_ACTIVE,
        GAME_MENU,
        GAME_WIN
    };
    SpriteRenderer* Renderer;
    GameState State;
    bool Keys[1024];
    unsigned int Width, Height;
    std::vector<GameLevel> Levels;
    unsigned int Level;

    Game(unsigned int width, unsigned int height);
    ~Game();

    void Init();
    void ProcessInput(float dt);
    void Update(float dt);
    void Render();
};

#endif
