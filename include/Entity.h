#pragma once
#include <SDL.h>
#include <string>
#include <iostream> // For logging
#include "Math/Vector2Int.h"
#include "AssetConfigs.h"

// Forward Declarations to avoid circular includes
class InputManager;
class TileMap;

struct Stats
{
    int maxHP = 10;
    int damage = 1;
    int armor = 0; // AC (Defense)
};

class Entity
{
public:
    Entity(int x, int y, const std::string &name, int spriteID, Stats stats);
    virtual ~Entity();

    // Virtual methods: Children (Player/Enemy) will override these
    virtual bool Update(float deltaTime, InputManager *input, TileMap *map) = 0; // Pure virtual = Must override

    // Base Render: Most entities just draw their sprite, so we implement it here
    virtual void Render(SDL_Renderer *renderer, SDL_Texture *texture, SDL_Rect camera);
    
    // --- COMBAT METHODS ---
    bool IsAlive() const { return currentHP > 0; }
    void Attack(Entity *target);
    void TakeDamage(int amount);
    
    // Getters
    Vector2Int GetPos() const { return pos; }
    std::string GetName() const { return name; }
    int GetHP() const { return currentHP; }
    int GetMaxHP() const { return baseStats.maxHP; }
    int GetDamage() const { return baseStats.damage; }
    int GetArmor() const { return baseStats.armor; }



protected:
    Vector2Int pos;
    std::string name;
    int spriteID;   // Which tile to draw from the AssetRegistry
    int size;       // Pixel size (usually 32)
    bool isVisible; // For later (Fog of War)

    Stats baseStats;
    int currentHP;
};