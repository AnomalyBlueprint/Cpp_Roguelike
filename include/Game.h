#pragma once
#include <SDL.h>
#include <iostream>
#include "Player.h"
#include "InputManager.h"
#include "TileMap.h"
#include "Entity.h"
#include "Enemy.h"

class Game
{
public:
    Game();
    ~Game();

    void Init(const char *title, int width, int height);

    // The Master Loop
    void Run();

    void Clean();
    bool IsRunning() { return isRunning; }

    TileMap *GetTileMap() { return level; }
    enum class GameState
    {
        PLAYERTURN,  // Waiting for W/A/S/D
        ENEMYTURN,   // Rats move
        ENVIRONMENT, // Traps, Fire, Spikes
        RESOLVE      // Buffs/Debuffs (Poison damage, etc.)
    };

private:
    // Internal Engine Steps
    void HandleEvents();
    void Update(float deltaTime);
    void FixedUpdate(); // For Physics/Fixed Logic
    void Render();

    GameState gameState;

    SDL_Rect camera;
    int mapWidth;
    int mapHeight;
    
    bool isRunning;
    SDL_Window *window;
    SDL_Renderer *renderer;

    // Subsystems
    Player *player;
    InputManager *inputManager;
    std::vector<Entity *> enemies;

    // Timing Variables
    Uint64 lastTime;
    double lag;                           // Accumulated time
    const double MS_PER_UPDATE = 16.6666; // 60 updates per second (approx)

    TileMap *level;
    SDL_Texture *tileset;
};