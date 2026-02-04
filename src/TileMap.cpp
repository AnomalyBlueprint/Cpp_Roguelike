#include "TileMap.h"
#include "TextureManager.h"

TileMap::TileMap(int width, int height, int tileSize)
    : width(width), height(height), tileSize(tileSize)
{
    // Initialize Registry ONCE when map is created
    AssetRegistry::Get().Initialize();
}

TileMap::~TileMap() {}

void TileMap::LoadLayers(const std::vector<int> &ground, const std::vector<int> &walls)
{
    if (ground.size() == width * height)
        groundLayer = ground;
    if (walls.size() == width * height)
        wallLayer = walls;
}

void TileMap::Render(SDL_Renderer *renderer, SDL_Texture *texture)
{
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {

            // 1. Determine Screen Position
            SDL_Rect destRect = {x * tileSize, y * tileSize, tileSize, tileSize};

            // 2. DRAW GROUND LAYER (Always Draw)
            int groundID = groundLayer[y * width + x];
            const TileData &groundData = AssetRegistry::Get().GetTile(groundID);
            TextureManager::Draw(texture, groundData.srcRect, destRect, renderer);

            // 3. DRAW WALL LAYER (Only if not 0/Empty)
            int wallID = wallLayer[y * width + x];
            if (wallID != 0)
            {
                const TileData &wallData = AssetRegistry::Get().GetTile(wallID);
                TextureManager::SetColor(texture, 180, 180, 180);
                TextureManager::Draw(texture, wallData.srcRect, destRect, renderer);
            }
        }
    }
    TextureManager::SetColor(texture, 255, 255, 255);
}

bool TileMap::IsWall(int x, int y)
{
    if (x < 0 || x >= width || y < 0 || y >= height)
        return true;

    // Check the Registry!
    int wallID = wallLayer[y * width + x];
    const TileData &data = AssetRegistry::Get().GetTile(wallID);

    // Return true only if the tile is explicitly NOT walkable
    return !data.isWalkable;
}