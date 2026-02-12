#include "Enemy.h"
#include <cstdlib> // For rand()
#include "TileMap.h"

// Convert DB Stats -> Entity Stats
Enemy::Enemy(int x, int y, const MonsterStats &dbStats)
    : Entity(x, y, dbStats.name, dbStats.spriteID, {dbStats.hp, dbStats.damage, 0}) // Assuming 0 armor for now
{
}

// ... Rest of Enemy Code ...

Enemy::~Enemy() {}

bool Enemy::Update(float deltaTime, InputManager *input, TileMap *map)
{
    // Simple AI: 50% chance to move randomly
    if (rand() % 100 < 50)
    {
        int moveX = (rand() % 3) - 1;
        int moveY = (rand() % 3) - 1;

        Vector2Int target = pos + Vector2Int(moveX * 32, moveY * 32);

        if (!map->IsWall(target.x / 32, target.y / 32))
        {
            pos = target;
        }
    }
    return true; // Enemy always finishes its turn
}