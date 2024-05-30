#include "internal/Game.h"
#include <filesystem>


Game::Game(unsigned int width, unsigned int height)
    : State(GAME_MENU), Keys(), Width(width), Height(height)
{
}

Game::~Game()
{
    delete Renderer;
}

TextRenderer* Text;

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

    Text = new TextRenderer(this->Width, this->Height);
    Text->Load(str_dir + "/assets/fonts/ocraext.TTF", 24);

    ResourceManager::LoadTexture((str_dir + "/assets/textures/tile_grass_1.png").c_str(), true, "tile_grass");
    ResourceManager::LoadTexture((str_dir + "/assets/textures/tile_wall.png").c_str(), true, "tile_wall");
    ResourceManager::LoadTexture((str_dir + "/assets/textures/tile_hole.png").c_str(), true, "tile_hole");
    ResourceManager::LoadTexture((str_dir + "/assets/textures/tile_filled_hole.png").c_str(), true, "tile_filled_hole");
    ResourceManager::LoadTexture((str_dir + "/assets/textures/tile_goal.png").c_str(), true, "tile_goal");
    ResourceManager::LoadTexture((str_dir + "/assets/textures/entity_player.png").c_str(), true, "entity_player");
    ResourceManager::LoadTexture((str_dir + "/assets/textures/entity_box.png").c_str(), true, "entity_box");
    ResourceManager::LoadTexture((str_dir + "/assets/textures/background.jpg").c_str(), false, "background");

    GameLevel lvl1;
    GameLevel lvl2;
    GameLevel lvl3;
    GameLevel lvl4;
    lvl1.Load((str_dir + "/levels/lvl1.lvl").c_str(), this->Width, this->Height);
    lvl2.Load((str_dir + "/levels/lvl2.lvl").c_str(), this->Width, this->Height);
    lvl3.Load((str_dir + "/levels/lvl3.lvl").c_str(), this->Width, this->Height);
    lvl4.Load((str_dir + "/levels/lvl4.lvl").c_str(), this->Width, this->Height);
    this->Levels = {lvl1, lvl2, lvl3, lvl4};
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

    if (this->State == GAME_MENU)
    {
        if (this->Keys[GLFW_KEY_ENTER] && !KeysProcessed[GLFW_KEY_ENTER])
        {
            this->State = GAME_ACTIVE;
            KeysProcessed[GLFW_KEY_ENTER] = true;
        }
        if (this->Keys[GLFW_KEY_W] && !KeysProcessed[GLFW_KEY_W])
        {   
            this->Level = (this->Level + 1) % 4;
            KeysProcessed[GLFW_KEY_W] = true;
        }
        if (this->Keys[GLFW_KEY_S] && !KeysProcessed[GLFW_KEY_S])
        {
            if (this->Level > 0)
                --this->Level;
            else
                this->Level = 3;
            KeysProcessed[GLFW_KEY_S] = true;
        }
    }
    if (this->State == GAME_WIN)
    {
        if (this->Keys[GLFW_KEY_ENTER])
        {
            this->KeysProcessed[GLFW_KEY_ENTER] = true;
            this->Level = (Level+1)%Levels.size();
            ResetLevel();
            this->State = GAME_MENU;
        }
    }
}
void Game::Update(float dt)
{
    if (this->State == GAME_ACTIVE && this->Levels[this->Level].IsCompleted())
    {
        this->State = GAME_WIN;
    }
}

void Game::Render()
{
    if (this->State == GAME_ACTIVE || this->State == GAME_MENU || GAME_WIN)
    {
        Texture2D tex = ResourceManager::GetTexture("background");
        Renderer->drawBackground(tex, glm::vec2(this->Width, this->Height));
        this->Levels[this->Level].Draw(*Renderer);
    }
    if (this->State == GAME_MENU)
    {
        Text->RenderText("Press ENTER to start", 250.0f, Height / 2, 1.0f);
        Text->RenderText("Press W or S to select level", 245.0f, Height / 2 + 20.0f, 0.75f);
    }
    if (this->State == GAME_WIN)
    {
        Text->RenderText(
            "Level Complete!", 280.0, Height / 2 - 20.0, 1.0, glm::vec3(0.0, 1.0, 0.0)
        );
        Text->RenderText(
            "Press ENTER to choose the next level", 130.0, Height / 2 + 10, 1.0, glm::vec3(1.0, 1.0, 0.0)
        );
    }
}
void Game::ResetLevel()
{
    std::filesystem::path full_path(__FILE__);
    std::filesystem::path dir = full_path.parent_path().parent_path();
    std::string str_dir = dir.string();

    if (this->Level == 0)
        this->Levels[0].Load((str_dir + "/levels/lvl1.lvl").c_str(), this->Width, this->Height / 2);
    else if (this->Level == 1)
        this->Levels[1].Load((str_dir + "/levels/lvl2.lvl").c_str(), this->Width, this->Height / 2);
    else if (this->Level == 2)
        this->Levels[2].Load((str_dir + "/levels/lvl3.lvl").c_str(), this->Width, this->Height / 2);
    else if (this->Level == 3)
        this->Levels[3].Load((str_dir + "/levels/lvl4.lvl").c_str(), this->Width, this->Height / 2);
}
