#include "Player.h"
#include "TileMap.h"
#include "EntityManager.h"

Player::Player(int x, int y) : Entity(x, y, "Hero", 20, {20, 4, 1}), TILE_SIZE(32) {}

Player::~Player() {}

bool Player::Update(float deltaTime, InputManager *input, TileMap *map)
{
    Vector2Int targetPos = pos;

    if (input->GetKeyPressed(SDLK_w))
        targetPos.y -= TILE_SIZE;
    else if (input->GetKeyPressed(SDLK_s))
        targetPos.y += TILE_SIZE;
    else if (input->GetKeyPressed(SDLK_a))
        targetPos.x -= TILE_SIZE;
    else if (input->GetKeyPressed(SDLK_d))
        targetPos.x += TILE_SIZE;

    // If no key pressed, return false (Turn not over)
    if (targetPos.x == pos.x && targetPos.y == pos.y)
        return false;

    // If we tried to move into a wall, we didn't actually move.
    int gridX = targetPos.x / TILE_SIZE;
    int gridY = targetPos.y / TILE_SIZE;

    Entity *target = EntityManager::Get().GetEntityAt(targetPos.x, targetPos.y);

    if (target)
    {
        Attack(target);
        return true; // Turn used to attack
    }

    if (!map->IsWall(gridX, gridY))
    {
        pos = targetPos;
        return true; // WE MOVED! End turn.
    }

    return false; // Hit a wall, turn not over.
}