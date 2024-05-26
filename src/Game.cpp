#include "internal/Game.h"
#include "internal/resource_manager.h"
#include "internal/SpriteRenderer.h"
#include "internal/GameLevel.h"

#include <filesystem>

// Game-related State data
SpriteRenderer* Renderer;

Game::Game(unsigned int width, unsigned int height)
    : State(GAME_ACTIVE), Keys(), Width(width), Height(height)
{

}

Game::~Game()
{
    delete Renderer;
}

void Game::Init()
{
    //configure file paths
    std::filesystem::path full_path(__FILE__);
    std::filesystem::path dir = full_path.parent_path().parent_path();
    std::string str_dir = dir.string();
    //shader paths
    std::string vertexPath = str_dir + "/shaders/sprite.vrt";
    std::string fragmentPath = str_dir + "/shaders/sprite.frg";

    ResourceManager::LoadShader(vertexPath.c_str(), fragmentPath.c_str(), nullptr, "sprite");
    // configure shaders
    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(this->Width),
        static_cast<float>(this->Height), 0.0f, -1.0f, 1.0f);
    ResourceManager::GetShader("sprite").Use().SetInteger("image", 0);
    ResourceManager::GetShader("sprite").SetMatrix4("projection", projection);

    // set render-specific controls
    Shader shader = ResourceManager::GetShader("sprite");
    Camera2D camera(glm::vec2(0.0f, 0.0f), 1.0f, this->Width, this->Height);

    Renderer = new SpriteRenderer(shader, camera);
    this->Renderer = Renderer;
    // load textures
    ResourceManager::LoadTexture((str_dir + "/assets/textures/default_texture.png").c_str(), true, "default_texture");

    ResourceManager::LoadTexture((str_dir + "/assets/textures/tile_none.png").c_str(), true, "tile_none");
    ResourceManager::LoadTexture((str_dir + "/assets/textures/tile_grass_1.png").c_str(), true, "tile_grass_1");
    ResourceManager::LoadTexture((str_dir + "/assets/textures/tile_grass_2.png").c_str(), true, "tile_grass_2");
    ResourceManager::LoadTexture((str_dir + "/assets/textures/tile_grass_3.png").c_str(), true, "tile_grass_3");
    ResourceManager::LoadTexture((str_dir + "/assets/textures/tile_wall.png").c_str(), true, "tile_wall");
    ResourceManager::LoadTexture((str_dir + "/assets/textures/tile_hole.png").c_str(), true, "tile_hole");
    ResourceManager::LoadTexture((str_dir + "/assets/textures/tile_filled_hole.png").c_str(), true, "tile_filled_hole");

    ResourceManager::LoadTexture((str_dir + "/assets/textures/entity_box.png").c_str(), true, "entity_box");
    ResourceManager::LoadTexture((str_dir + "/assets/textures/entity_player.png").c_str(), true, "entity_player");

    ResourceManager::LoadTexture((str_dir + "/assets/textures/background.jpg").c_str(), false, "background");
    // load levels
    GameLevel lvl1; lvl1.Load((str_dir + "/levels/lvl1.lvl").c_str(), this->Width, this->Height);
    this->Levels.push_back(lvl1);
    this->Level = 0;
}

void Game::Update(float dt)
{

}

void Game::ProcessInput(float dt)
{
    if(this->State == GAME_ACTIVE)
    {
        // move playerboard
        if (this->Keys[GLFW_KEY_UP])
        {
            Renderer->camera.processKeyboard(UP, dt);
        }
        if (this->Keys[GLFW_KEY_LEFT])
        {
            Renderer->camera.processKeyboard(LEFT, dt);
        }
        if (this->Keys[GLFW_KEY_DOWN])
        {
            Renderer->camera.processKeyboard(DOWN, dt);
        }
        if (this->Keys[GLFW_KEY_RIGHT])
        {
            Renderer->camera.processKeyboard(RIGHT, dt);
        }
    }
}
void Game::Render()
{
    if (this->State == GAME_ACTIVE)
    {
        // draw background
        Texture2D background = ResourceManager::GetTexture("background");
        Renderer->drawBackground(background, glm::vec2(this->Width, this->Height));
        // draw level
        this->Levels[this->Level].draw(*Renderer);
    }
}