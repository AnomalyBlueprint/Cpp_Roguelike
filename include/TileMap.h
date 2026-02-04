#pragma once
#include <SDL.h>
#include <vector>
#include "AssetConfigs.h" // Include our Registry

class TileMap
{
public:
    TileMap(int width, int height, int tileSize);
    ~TileMap();

    // Now we load two layers!
    void LoadLayers(const std::vector<int> &ground, const std::vector<int> &walls);

    void Render(SDL_Renderer *renderer, SDL_Texture *texture);
    bool IsWall(int x, int y);

private:
    std::vector<int> groundLayer; // Always walkable (Grass, Dirt)
    std::vector<int> wallLayer;   // Collidable (Walls, Trees)

    int width, height;
    int tileSize;
};