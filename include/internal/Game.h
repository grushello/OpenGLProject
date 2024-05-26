#include <glad/glad.h>
#include<GLFW/glfw3.h>

#include<vector>

#include "internal/Camera.h"
#include<internal/GameLevel.h>

enum GameState 
{
    GAME_ACTIVE,
    GAME_MENU,
    GAME_WIN
};

class Game
{
public:
    // active renderer
    SpriteRenderer* Renderer;
    // game state
    GameState    State;
    bool         Keys[1024];
    unsigned int Width, Height;
    // levels
    std::vector<GameLevel> Levels;
    unsigned int           Level;
    // constructor/destructor
    Game(unsigned int width, unsigned int height);
    ~Game();
    // initialize game state (load all shaders/textures/levels)
    void Init();
    // game loop
    void ProcessInput(float dt);
    void scrollCallback(double yoffset);
    void Update(float dt);
    void Render();
};
