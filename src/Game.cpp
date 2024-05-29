#include "internal/Game.h"
#include <filesystem>


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
    std::filesystem::path full_path(__FILE__);
    std::filesystem::path dir = full_path.parent_path().parent_path();
    std::string str_dir = dir.string();

    std::string vertexPath = str_dir + "/shaders/sprite.vrt";
    std::string fragmentPath = str_dir + "/shaders/sprite.frg";
    ResourceManager::LoadShader(vertexPath.c_str(), fragmentPath.c_str(), nullptr, "sprite");

    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(this->Width),
        static_cast<float>(this->Height), 0.0f, -1.0f, 1.0f);
    ResourceManager::GetShader("sprite").Use().SetInteger("image", 0);
    ResourceManager::GetShader("sprite").SetMatrix4("projection", projection);

    Shader shader = ResourceManager::GetShader("sprite");
    Camera2D camera(glm::vec2(0.0f, 0.0f), 1.0f, this->Width, this->Height);

    Renderer = new SpriteRenderer(shader, camera);

    ResourceManager::LoadTexture((str_dir + "/assets/textures/default_texture.png").c_str(), true, "default_texture");

    ResourceManager::LoadTexture((str_dir + "/assets/textures/tile_grass_1.png").c_str(), true, "tile_grass");
    ResourceManager::LoadTexture((str_dir + "/assets/textures/tile_wall.png").c_str(), true, "tile_wall");
    ResourceManager::LoadTexture((str_dir + "/assets/textures/tile_hole.png").c_str(), true, "tile_hole");
    ResourceManager::LoadTexture((str_dir + "/assets/textures/tile_filled_hole.png").c_str(), true, "tile_filled_hole");
    ResourceManager::LoadTexture((str_dir + "/assets/textures/tile_goal.png").c_str(), true, "tile_goal");
    ResourceManager::LoadTexture((str_dir + "/assets/textures/entity_player.png").c_str(), true, "entity_player");
    ResourceManager::LoadTexture((str_dir + "/assets/textures/entity_box.png").c_str(), true, "entity_box");
    ResourceManager::LoadTexture((str_dir + "/assets/textures/background.jpg").c_str(), false, "background");

    // Load levels
    GameLevel lvl1;
    lvl1.Load((str_dir + "/levels/lvl1.lvl").c_str(), this->Width, this->Height);
    this->Levels.push_back(lvl1);
    this->Level = 0;
}

void Game::ProcessInput(float dt)
{
    if (this->State == GAME_ACTIVE)
    {
        static double lastMoveTime = glfwGetTime();

        double currentTime = glfwGetTime();
        double elapsedTime = currentTime - lastMoveTime;

        if (elapsedTime >= 0.2)
        {
            if (this->Keys[GLFW_KEY_UP])
            {
                this->Levels[this->Level].ProcessPlayerMovement(PLAYER_UP);
                lastMoveTime = currentTime;
            }
            else if (this->Keys[GLFW_KEY_LEFT])
            {
                this->Levels[this->Level].ProcessPlayerMovement(PLAYER_LEFT);
                lastMoveTime = currentTime;
            }
            else if (this->Keys[GLFW_KEY_DOWN])
            {
                this->Levels[this->Level].ProcessPlayerMovement(PLAYER_DOWN);
                lastMoveTime = currentTime;
            }
            else if (this->Keys[GLFW_KEY_RIGHT])
            {
                this->Levels[this->Level].ProcessPlayerMovement(PLAYER_RIGHT);
                lastMoveTime = currentTime;
            }
        }

        if (this->Keys[GLFW_KEY_W])
        {
            Renderer->camera.processKeyboard(CAMERA_UP, dt);
        }
        if (this->Keys[GLFW_KEY_A])
        {
            Renderer->camera.processKeyboard(CAMERA_LEFT, dt);
        }
        if (this->Keys[GLFW_KEY_S])
        {
            Renderer->camera.processKeyboard(CAMERA_DOWN, dt);
        }
        if (this->Keys[GLFW_KEY_D])
        {
            Renderer->camera.processKeyboard(CAMERA_RIGHT, dt);
        }
    }
}
void Game::Update(float dt)
{
    return;
}

void Game::Render()
{
    if (this->State == GAME_ACTIVE)
    {
        Texture2D tex = ResourceManager::GetTexture("background");
        Renderer->drawBackground(tex, glm::vec2(this->Width, this->Height));
        this->Levels[this->Level].Draw(*Renderer);
    }
}
