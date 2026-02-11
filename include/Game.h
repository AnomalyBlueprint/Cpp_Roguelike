#pragma once
#include <SDL.h>
#include <iostream>
#include "Player.h"
#include "InputManager.h"
#include "TileMap.h"

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
private:
    // Internal Engine Steps
    void HandleEvents();
    void Update(float deltaTime);
    void FixedUpdate(); // For Physics/Fixed Logic
    void Render();

    SDL_Rect camera;
    int mapWidth;
    int mapHeight;
    
    bool isRunning;
    SDL_Window *window;
    SDL_Renderer *renderer;

    // Subsystems
    Player *player;
    InputManager *inputManager;

    // Timing Variables
    Uint64 lastTime;
    double lag;                           // Accumulated time
    const double MS_PER_UPDATE = 16.6666; // 60 updates per second (approx)

    TileMap *level;
    SDL_Texture *tileset;
};