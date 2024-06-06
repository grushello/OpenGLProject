#include "internal/Game.h"
#include <filesystem>
#include <fstream>
#include <sstream>
#include <string>

Game::Game(unsigned int width, unsigned int height)
{
    this->gameData.setGameState(gameData.GAME_MENU);
    Width = width;
    Height = height;
}

Game::~Game()
{
    delete Renderer;
}

TextRenderer* Text;

void Game::Load()
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
    ResourceManager::LoadTexture((str_dir + "/assets/textures/tile_big_hole.png").c_str(), true, "tile_big_hole");
    ResourceManager::LoadTexture((str_dir + "/assets/textures/tile_filled_hole.png").c_str(), true, "tile_filled_hole");
    ResourceManager::LoadTexture((str_dir + "/assets/textures/tile_goal.png").c_str(), true, "tile_goal");
    ResourceManager::LoadTexture((str_dir + "/assets/textures/entity_player.png").c_str(), true, "entity_player");
    ResourceManager::LoadTexture((str_dir + "/assets/textures/entity_box.png").c_str(), true, "entity_box");
    ResourceManager::LoadTexture((str_dir + "/assets/textures/background.jpg").c_str(), false, "background");

    GameLevel lvl0;
    GameLevel lvl1;
    GameLevel lvl2;
    GameLevel lvl3;
    GameLevel lvl4;
    lvl0.Load((str_dir + "/levels/lvl0.lvl").c_str(), this->Width, this->Height);
    lvl1.Load((str_dir + "/levels/lvl1.lvl").c_str(), this->Width, this->Height);
    lvl2.Load((str_dir + "/levels/lvl2.lvl").c_str(), this->Width, this->Height);
    lvl3.Load((str_dir + "/levels/lvl3.lvl").c_str(), this->Width, this->Height);
    lvl4.Load((str_dir + "/levels/lvl4.lvl").c_str(), this->Width, this->Height);
    this->Levels = { lvl0, lvl1, lvl2, lvl3, lvl4 };
    this->gameData.LoadSave();
}

void Game::ProcessInput(float dt)
{
    if (this->gameData.getGameState() == gameData.GAME_ACTIVE
        || this->gameData.getGameState() == gameData.GAME_MENU)
    {

        if (this->Keys[GLFW_KEY_UP])
        {
            Renderer->camera.processKeyboard(CAMERA_UP, dt);
            KeysProcessed[GLFW_KEY_UP] = true;
        }
        if (this->Keys[GLFW_KEY_LEFT])
        {
            Renderer->camera.processKeyboard(CAMERA_LEFT, dt);
            KeysProcessed[GLFW_KEY_LEFT] = true;
        }
        if (this->Keys[GLFW_KEY_DOWN])
        {
            Renderer->camera.processKeyboard(CAMERA_DOWN, dt);
            KeysProcessed[GLFW_KEY_DOWN] = true;
        }
        if (this->Keys[GLFW_KEY_RIGHT])
        {
            Renderer->camera.processKeyboard(CAMERA_RIGHT, dt);
            KeysProcessed[GLFW_KEY_RIGHT] = true;
        }
    }
    if (this->gameData.getGameState() == gameData.GAME_ACTIVE)
    {
        static double lastMoveTime = glfwGetTime();

        double currentTime = glfwGetTime();
        double elapsedTime = currentTime - lastMoveTime;

		if (this->Keys[GLFW_KEY_W] && !KeysProcessed[GLFW_KEY_W])
		{
			this->Levels[this->gameData.getCurrentLevel()].ProcessGameLogic(PLAYER_UP);
			lastMoveTime = currentTime;
            KeysProcessed[GLFW_KEY_W] = true;
		}
		else if (this->Keys[GLFW_KEY_A] && !KeysProcessed[GLFW_KEY_A])
		{
			this->Levels[this->gameData.getCurrentLevel()].ProcessGameLogic(PLAYER_LEFT);
			lastMoveTime = currentTime;
            KeysProcessed[GLFW_KEY_A] = true;
		}
		else if (this->Keys[GLFW_KEY_S] && !KeysProcessed[GLFW_KEY_S])
		{
			this->Levels[this->gameData.getCurrentLevel()].ProcessGameLogic(PLAYER_DOWN);
			lastMoveTime = currentTime;
            KeysProcessed[GLFW_KEY_S] = true;
		}
		else if (this->Keys[GLFW_KEY_D] && !KeysProcessed[GLFW_KEY_D])
		{
			this->Levels[this->gameData.getCurrentLevel()].ProcessGameLogic(PLAYER_RIGHT);
			lastMoveTime = currentTime;
            KeysProcessed[GLFW_KEY_D] = true;
		}
        if (this->Keys[GLFW_KEY_ESCAPE] && !KeysProcessed[GLFW_KEY_ESCAPE])
        {
            this->ResetLevel();
            this->gameData.setGameState(gameData.GAME_MENU);
            KeysProcessed[GLFW_KEY_ESCAPE] = true;
        }
        if (this->Keys[GLFW_KEY_R] && !KeysProcessed[GLFW_KEY_R])
        {
            this->ResetLevel();
            KeysProcessed[GLFW_KEY_R] = true;
        }
    }

    if (this->gameData.getGameState() == gameData.GAME_MENU)
    {
        if (this->Keys[GLFW_KEY_ENTER] && !KeysProcessed[GLFW_KEY_ENTER])
        {
            this->gameData.setGameState(gameData.GAME_ACTIVE);
            KeysProcessed[GLFW_KEY_ENTER] = true;
        }
        if (this->Keys[GLFW_KEY_W] && !KeysProcessed[GLFW_KEY_W])
        {   
            this->gameData.setCurrentLevel((this->gameData.getCurrentLevel() + 1) % (gameData.getLevelsPassed()+1));
            KeysProcessed[GLFW_KEY_W] = true;
        }
        if (this->Keys[GLFW_KEY_S] && !KeysProcessed[GLFW_KEY_S])
        {
            if (this->gameData.getCurrentLevel() > 0)
                this->gameData.setCurrentLevel(this->gameData.getCurrentLevel() - 1);
            else
                this->gameData.setCurrentLevel(gameData.getLevelsPassed());
            KeysProcessed[GLFW_KEY_S] = true;
        }
    }
    if (this->gameData.getGameState() == gameData.GAME_WIN)
    {
        if (this->Keys[GLFW_KEY_ENTER])
        {
            this->KeysProcessed[GLFW_KEY_ENTER] = true;
            ResetLevel();
            this->gameData.setCurrentLevel(1 + this->gameData.getCurrentLevel());
            ResetLevel();
            this->gameData.setGameState(gameData.GAME_MENU);
        }
    }
    if (this->gameData.getGameState() == gameData.GAME_FINISHED)
    {
        if (this->Keys[GLFW_KEY_ENTER] && !this->KeysProcessed[GLFW_KEY_ENTER])
        {
            ResetLevel();
            this->gameData.setGameState(gameData.GAME_MENU);
            this->KeysProcessed[GLFW_KEY_ENTER] = true;
        }
        else if (this->Keys[GLFW_KEY_BACKSPACE] && !this->KeysProcessed[GLFW_KEY_BACKSPACE])
        {
            this->gameData.setGameState(gameData.GAME_MENU);
            gameData.DeleteSave();
            this->Load();
            this->KeysProcessed[GLFW_KEY_BACKSPACE] = true;
        }
    }
}
void Game::Update(float dt)
{
    if (this->gameData.getGameState() == gameData.GAME_ACTIVE
        && this->Levels[this->gameData.getCurrentLevel()].IsCompleted()
        && gameData.getCurrentLevel() == this->Levels.size() - 1)
    {
        this->gameData.setGameState(gameData.GAME_FINISHED);
        gameData.Save();
    }
    else if (this->gameData.getGameState() == gameData.GAME_ACTIVE 
        && this->Levels[this->gameData.getCurrentLevel()].IsCompleted())
    {
        this->gameData.setGameState(gameData.GAME_WIN);
        if (gameData.getCurrentLevel() == gameData.getLevelsPassed())
            gameData.setLevelsPassed(gameData.getLevelsPassed() + 1);
        gameData.Save();
    }
}

void Game::Render()
{
    if (this->gameData.getGameState() == gameData.GAME_FINISHED)
    {
        Texture2D tex = ResourceManager::GetTexture("background");
        Renderer->drawBackground(tex, glm::vec2(this->Width, this->Height));

        Text->RenderText("You have finished the game!!! ", 95.0f, Height / 2 - 40.0f, 1.5f, glm::vec3(1.0f, 1.0f, 0.0f));
        Text->RenderText("Press ENTER to return back to levels", 95.0f, Height / 2, 0.75f);
        Text->RenderText("Press BACKSPACE to start from the beginning", 95.0f, Height / 2 + 20.0f, 0.75f);
        Text->RenderText("Here are some of your stats:", 95.0f, Height / 2 + 55.0f, 1.0f, glm::vec3(0.0f, 1.0f, 0.0f));
        Text->RenderText("Tiles Traveled: " + std::to_string(gameData.getTilesTraveled()), 95.0f, Height / 2 + 80.0f, 0.75f);
        Text->RenderText("Boxes Moved: " + std::to_string(gameData.getBoxesMoved()), 95.0f, Height / 2 + 100.0f, 0.75f);
        Text->RenderText("Holes Filled: " + std::to_string(gameData.getHolesFilled()), 95.0f, Height / 2 + 120.0f, 0.75f);
    }
    if (this->gameData.getGameState() == gameData.GAME_ACTIVE 
        || this->gameData.getGameState() == gameData.GAME_MENU 
        || this->gameData.getGameState() == gameData.GAME_WIN)
    {
        Texture2D tex = ResourceManager::GetTexture("background");
        Renderer->drawBackground(tex, glm::vec2(this->Width, this->Height));
        this->Levels[this->gameData.getCurrentLevel()].Draw(*Renderer);
    }
    if (this->gameData.getGameState() == gameData.GAME_MENU)
    {
        Text->RenderText("Press ENTER to start", 250.0f, Height / 2, 1.0f);
        Text->RenderText("Press W or S to select level", 245.0f, Height / 2 + 20.0f, 0.75f);
    }
    if (this->gameData.getGameState() == gameData.GAME_WIN)
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

    if (this->gameData.getCurrentLevel() == 0)
    {
        this->Levels[0].Load((str_dir + "/levels/lvl0.lvl").c_str(), this->Width, this->Height / 2);
    }
    else if (this->gameData.getCurrentLevel()== 1)
    {
        this->Levels[1].Load((str_dir + "/levels/lvl1.lvl").c_str(), this->Width, this->Height / 2);
    }
    else if (this->gameData.getCurrentLevel()== 2)
    {
        this->Levels[2].Load((str_dir + "/levels/lvl2.lvl").c_str(), this->Width, this->Height / 2);
    }
    else if (this->gameData.getCurrentLevel()== 3)
    {    
        this->Levels[3].Load((str_dir + "/levels/lvl3.lvl").c_str(), this->Width, this->Height / 2);
    }
    else if (this->gameData.getCurrentLevel() == 4)
    {
        this->Levels[4].Load((str_dir + "/levels/lvl4.lvl").c_str(), this->Width, this->Height / 2);
    }
}