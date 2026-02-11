#pragma once
#include <SDL.h>
#include <string>
#include "Math/Vector2Int.h"
#include "AssetConfigs.h"

// Forward Declarations to avoid circular includes
class InputManager;
class TileMap;

class Entity
{
public:
    Entity(int x, int y, const std::string &name, int spriteID);
    virtual ~Entity();

    // Virtual methods: Children (Player/Enemy) will override these
    virtual bool Update(float deltaTime, InputManager *input, TileMap *map) = 0; // Pure virtual = Must override

    // Base Render: Most entities just draw their sprite, so we implement it here
    virtual void Render(SDL_Renderer *renderer, SDL_Texture *texture, SDL_Rect camera);

    // Getters
    Vector2Int GetPos() const { return pos; }
    std::string GetName() const { return name; }

protected:
    Vector2Int pos;
    std::string name;
    int spriteID;   // Which tile to draw from the AssetRegistry
    int size;       // Pixel size (usually 32)
    bool isVisible; // For later (Fog of War)
};