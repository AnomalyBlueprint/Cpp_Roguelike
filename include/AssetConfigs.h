#pragma once
#include <SDL.h>
#include <unordered_map>
#include <string>

struct TileData
{
    int id;
    std::string name;
    SDL_Rect srcRect; // We still store Rect for the renderer
    bool isWalkable;
};

class AssetRegistry
{
public:
    static AssetRegistry &Get()
    {
        static AssetRegistry instance;
        return instance;
    }

    // --- CONFIGURATION ---
    const int ATLAS_TILE_SIZE = 8; // Kenney Micro Roguelike is 8x8

    void Initialize()
    {
        // --- HOW TO USE ---
        // RegisterTile(ID, "Name", Grid_X, Grid_Y, Walkable);
        // Grid_X = Column (0 is far left)
        // Grid_Y = Row (0 is top)

        RegisterTile(0, "Grass_1", 4, 4, true);
        RegisterTile(1, "Grass_2", 5, 4, true);
        RegisterTile(2, "Flowers", 4, 8, true);

        RegisterTile(10, "Wall_Top", 1, 0, false);
        RegisterTile(11, "Wall_Left", 0, 1, false);
        RegisterTile(12, "Wall_Right", 3, 1, false);
        RegisterTile(13, "Wall_Bottom", 1, 0, false);

        // CORNERS
        RegisterTile(14, "Corner_TL", 0, 0, false);
        RegisterTile(15, "Corner_TR", 3, 0, false);
        RegisterTile(16, "Corner_BL", 0, 2, false);
        RegisterTile(17, "Corner_BR", 3, 2, false);

        // VOID/FILLER
        RegisterTile(99, "Void", 1, 1, false);

        RegisterTile(20, "Hero", 4, 0, false);
        RegisterTile(21, "Skeleton_Warrior", 10, 0, false);
        RegisterTile(22, "Orc_Grunt", 11, 0, false);
        RegisterTile(23, "Crab", 12, 0, false);
        RegisterTile(24, "Floating_Eye", 13, 0, false);
        RegisterTile(25, "Swamp_Snake", 4, 1, false);
        RegisterTile(26, "Wild_Dog", 5, 1, false);
        RegisterTile(27, "Cave_Rat", 6, 1, false);
        RegisterTile(28, "Wood_Bug", 7, 1, false);
        RegisterTile(29, "Slime_Phantom", 8, 1, false);
        RegisterTile(30, "Ghost", 9, 1, false);
        RegisterTile(31, "Tortoise", 10, 1, false);
        RegisterTile(32, "Octopus", 11, 0, false);
        RegisterTile(33, "Screaming_Mandrake", 12, 0, false);

        bitmaskMap[12] = 14; 
        bitmaskMap[10] = 15; 
        bitmaskMap[5] = 16;  
        bitmaskMap[3] = 17;  

        bitmaskMap[6] = 10; 
        bitmaskMap[9] = 11;
    }




    const TileData &GetTile(int id)
    {
        if (registry.find(id) != registry.end())
        {
            return registry[id];
        }
        return registry[0]; // Fallback to Grass
    }

    int GetAutoTile(int mask)
    {
        if (bitmaskMap.find(mask) != bitmaskMap.end())
        {
            return bitmaskMap[mask];
        }
        return 10; // Default to basic wall if unknown
    }

private:
    std::unordered_map<int, TileData> registry;

    // UPDATED: Now takes (gridX, gridY) instead of SDL_Rect
    void RegisterTile(int id, std::string name, int gridX, int gridY, bool walkable)
    {
        TileData data;
        data.id = id;
        data.name = name;
        data.isWalkable = walkable;

        // AUTOMATIC MATH: Calculate pixels here so you don't have to!
        data.srcRect.x = gridX * ATLAS_TILE_SIZE;
        data.srcRect.y = gridY * ATLAS_TILE_SIZE;
        data.srcRect.w = ATLAS_TILE_SIZE;
        data.srcRect.h = ATLAS_TILE_SIZE;

        registry[id] = data;
    }

    std::unordered_map<int, int> bitmaskMap;

    AssetRegistry() {}
};