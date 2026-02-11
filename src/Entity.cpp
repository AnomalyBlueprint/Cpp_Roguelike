#include "Entity.h"
#include "TextureManager.h"

Entity::Entity(int x, int y, const std::string &name, int spriteID)
    : pos(x, y), name(name), spriteID(spriteID), size(32), isVisible(true) {}

Entity::~Entity() {}

void Entity::Render(SDL_Renderer *renderer, SDL_Texture *texture, SDL_Rect camera)
{
    if (!isVisible)
        return;

    // Camera Transform
    SDL_Rect destRect = {
        pos.x - camera.x,
        pos.y - camera.y,
        size,
        size};

    // Bounds Check: Don't draw if off-screen (Simple Optimization)
    if (destRect.x < -size || destRect.y < -size ||
        destRect.x > 800 || destRect.y > 600)
    {
        return;
    }

    // Get Sprite Info
    const TileData &data = AssetRegistry::Get().GetTile(spriteID);

    if (texture)
    {
        TextureManager::Draw(texture, data.srcRect, destRect, renderer);
    }
    else
    {
        // Fallback: Pink Box
        SDL_SetRenderDrawColor(renderer, 255, 0, 255, 255);
        SDL_RenderFillRect(renderer, &destRect);
    }
}