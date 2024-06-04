#ifndef GAMEDATA_H
#define GAMEDATA_H

class GameData
{
public:
    enum GameState
    {
        GAME_ACTIVE,
        GAME_MENU,
        GAME_WIN
    };

private:
	unsigned int LevelsPassed = 0;
	unsigned int CurrentLevel = 0;
	unsigned int TilesTraveled = 0;
	GameState gameState = GAME_ACTIVE;
public:

    static GameData& getInstance()
    {
        static GameData instance;
        return instance;
    }

    int getTilesTraveled() const;
    void setTilesTraveled(int newValue);

    int getCurrentLevel() const;
    void setCurrentLevel(int newValue);

    int getLevelsPassed() const;
    void setLevelsPassed(int newValue);

    GameState getGameState() const;
    void setGameState(GameState newValue);

    void Save() const;
    void LoadSave();
    void DeleteSave();
private:
    GameData() : LevelsPassed(0), CurrentLevel(0), TilesTraveled(0) {}
    GameData(const GameData&) = delete;
    GameData& operator=(const GameData&) = delete;
};
#endif