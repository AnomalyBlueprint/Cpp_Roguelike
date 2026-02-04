#pragma once
#include <SDL.h>
#include "GameMath.h"
#include "InputManager.h" // Player needs to know Input exists

class TileMap; // Forward Declaration

class Player
{
public:
    Player(float x, float y);
    ~Player();

    // Removed HandleInput(SDL_Event). Replaced with InputManager dependency.
    void Update(float deltaTime, InputManager *input, TileMap *map);
    void Render(SDL_Renderer *renderer, SDL_Texture *texture);

private:
    Vector2Int pos;
    int size;
    const int TILE_SIZE = 32;
};