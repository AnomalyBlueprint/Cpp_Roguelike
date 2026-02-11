#include "Enemy.h"
#include <cstdlib> // For rand()
#include "TileMap.h"

Enemy::Enemy(int x, int y, const std::string &name, int spriteID)
    : Entity(x, y, name, spriteID) {}

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