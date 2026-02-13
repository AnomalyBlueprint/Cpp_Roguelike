#include "Entity.h"
#include "TextureManager.h"
#include "ParticleSystem.h"
#include "UIManager.h"

Entity::Entity(int x, int y, const std::string &name, int spriteID, Stats stats)
    : pos(x, y), name(name), spriteID(spriteID), size(32), isVisible(true), baseStats(stats)
{
    currentHP = baseStats.maxHP;
}

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

void Entity::Attack(Entity *target)
{
    if (!target)
        return;

    std::string msg = "[Combat] " + name + " attacks " + target->GetName() + "!";
    UIManager::Get().AddMessage(msg);

    // Basic Formula: Damage - Armor (Min 1 damage)
    int effectiveDamage = baseStats.damage - target->baseStats.armor;
    if (effectiveDamage < 1)
        effectiveDamage = 1;

    target->TakeDamage(effectiveDamage);
}

void Entity::TakeDamage(int amount)
{
    currentHP -= amount;
    SDL_Color color = {255, 50, 50, 255};
    std::string dmgText = "-" + std::to_string(amount);

    ParticleSystem::Get().EmitText(dmgText, pos.x, pos.y - 20, color);

    std::string msg = name + " took " + std::to_string(amount) + " damage.";
    UIManager::Get().AddMessage(msg); // Add to scroll log

    // 2. Spawn Blood Sprites (optional)
    // Assuming Tile ID 21 is a "dot" or blood sprite
    // ParticleSystem::Get().EmitSprite(21, pos.x, pos.y, 5, {255, 0, 0, 255});

    if (currentHP <= 0)
    {
        UIManager::Get().AddMessage(name + " died!", {200, 0, 0, 255}); // Red death message
    }
}