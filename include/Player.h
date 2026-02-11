#pragma once
#include "Entity.h" // Inherit from Entity
#include "InputManager.h"

class Player : public Entity
{
public:
    Player(int x, int y); // Simplified Constructor
    ~Player();

    bool Update(float deltaTime, InputManager *input, TileMap *map) override;

    // We DON'T override Render anymore! The base Entity class handles it.
    // (Unless you want special effects like a glowing aura)

private:
    const int TILE_SIZE = 32;
};