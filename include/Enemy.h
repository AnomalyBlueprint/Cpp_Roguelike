#pragma once
#include "Entity.h"
#include "MonsterConfig.h"

class Enemy : public Entity
{
public:
    Enemy(int x, int y, const MonsterStats &dbStats);
    ~Enemy();

    // We must implement this because it was pure virtual in Entity
    bool Update(float deltaTime, InputManager *input, TileMap *map) override;
};