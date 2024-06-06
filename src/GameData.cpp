#include "internal/GameData.h"
#include "fstream"
#include "sstream"
#include <filesystem>
#include <iostream>

int GameData::getTilesTraveled() const
{
    return this->TilesTraveled;
}

void GameData::setTilesTraveled(int newValue)
{
    this->TilesTraveled = newValue;
}

int GameData::getBoxesMoved() const
{
    return this->BoxesMoved;
}

void GameData::setBoxesMoved(int newValue)
{
    this->BoxesMoved = newValue;
}

int GameData::getHolesFilled() const
{
    return this->HolesFilled;
}

void GameData::setHolesFilled(int newValue)
{
    this->HolesFilled = newValue;
}

int GameData::getCurrentLevel() const
{
    return this->CurrentLevel;
}

void GameData::setCurrentLevel(int newValue)
{
    this->CurrentLevel = newValue;
}

int GameData::getLevelsPassed() const
{
    return this->LevelsPassed;
}

void GameData::setLevelsPassed(int newValue)
{
    this->LevelsPassed = newValue;
}

GameData::GameState GameData::getGameState() const
{
    return this->gameState;
}
void GameData::setGameState(GameData::GameState newValue)
{
    this->gameState = newValue;
}

void GameData::Save() const
{
    std::filesystem::path filePath(__FILE__);
    filePath = filePath.parent_path().parent_path() / "saves";
    if (!std::filesystem::exists(filePath)) 
    {
        if (!std::filesystem::create_directories(filePath)) 
        {
            std::cerr << "Failed to create directory: " << filePath << std::endl;
        }
    }
    filePath /= "save.sv";

    std::ofstream file(filePath.string(), std::ios::out);
    if (!file)
    {
        std::cerr << "Failed to open file for saving: " << filePath << "\n";
        return;
    }
    file << "LevelsPassed: " << LevelsPassed << "\n";
    file << "CurrentLevel: " << CurrentLevel << "\n";
    file << "TilesTraveled: " << TilesTraveled << "\n";
    file << "BoxesMoved: " << BoxesMoved << "\n";
    file << "HolesFilled: " << HolesFilled;
    file.close();
}

void GameData::DeleteSave()
{
    std::filesystem::path filePath(__FILE__);
    filePath = filePath.parent_path().parent_path() / "saves" / "save.sv";

    if (std::filesystem::exists(filePath))
    {
        if (!std::filesystem::remove(filePath))
        {
            std::cerr << "Failed to delete save file: " << filePath << "\n";
        }
    }
    else
    {
        std::cerr << "Trying to delete non-existing save file: " << filePath << "\n";
    }
}

void GameData::LoadSave()
{
    std::filesystem::path filePath(__FILE__);
    filePath = filePath.parent_path().parent_path() / "saves/save.sv";
    if (std::filesystem::exists(filePath))
    {
        std::ifstream inFile(filePath);
        if (!inFile)
        {
            std::cerr << "Failed to open file for loading: " << filePath << "\n";
            return;
        }

        std::string line = "";
        while (std::getline(inFile, line))
        {
            std::istringstream iss(line);
            std::string key;
            int value;
            iss >> key >> value;
            if (key == "LevelsPassed:")
            {
                LevelsPassed = value;
            }
            else if (key == "CurrentLevel:")
            {
                CurrentLevel = value;
            }
            else if (key == "TilesTraveled:")
            {
                TilesTraveled = value;
            }
            else if (key == "BoxesMoved:")
            {
                BoxesMoved = value;
            }
            else if (key == "HolesFilled:")
            {
                HolesFilled = value;
            }
        }
        inFile.close();
    }
    else
    {
        CurrentLevel = 0;
        LevelsPassed = 0;
        TilesTraveled = 0;
        BoxesMoved = 0;
        HolesFilled = 0;
    }
}