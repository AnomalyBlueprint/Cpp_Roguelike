#pragma once
#include "Entity.h"

class Enemy : public Entity
{
public:
    Enemy(int x, int y, const std::string &name, int spriteID);
    ~Enemy();

    // We must implement this because it was pure virtual in Entity
    bool Update(float deltaTime, InputManager *input, TileMap *map) override;
};