#include "Player.h"
#include "TileMap.h" 
#include "TextureManager.h"

Player::Player(float x, float y) : pos(x, y), size(32), TILE_SIZE(32) {}

Player::~Player() {}

void Player::Update(float deltaTime, InputManager *input, TileMap *map)
{
    // 1. Calculate the Target Position based on input
    Vector2Int targetPos = pos; // Start with current position

    if (input->GetKeyPressed(SDLK_w))
    {
        targetPos.y -= TILE_SIZE;
    }
    else if (input->GetKeyPressed(SDLK_s))
    {
        targetPos.y += TILE_SIZE;
    }
    else if (input->GetKeyPressed(SDLK_a))
    {
        targetPos.x -= TILE_SIZE;
    }
    else if (input->GetKeyPressed(SDLK_d))
    {
        targetPos.x += TILE_SIZE;
    }

    // 2. If we didn't try to move, do nothing
    if (targetPos.x == pos.x && targetPos.y == pos.y)
    {
        return;
    }

    // 3. Check Collision
    // Convert Pixel Coordinates -> Grid Coordinates
    int gridX = targetPos.x / TILE_SIZE;
    int gridY = targetPos.y / TILE_SIZE;

    // Ask the map: "Is this a wall?"
    if (!map->IsWall(gridX, gridY))
    {
        // It's safe! Commit the move.
        pos = targetPos;
    }
}

void Player::Render(SDL_Renderer *renderer, SDL_Texture *texture, SDL_Rect camera)
{
    SDL_Rect destRect = {
        (int)pos.x - camera.x,
        (int)pos.y - camera.y,
        size,
        size};

    SDL_Rect srcRect = {8*5, 0, 8, 8};

    if (texture)
    {
        TextureManager::Draw(texture, srcRect, destRect, renderer);
    }
    else
    {
        // Fallback if texture missing
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_RenderFillRect(renderer, &destRect);
    }
}