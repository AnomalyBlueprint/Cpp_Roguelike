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
        // Initialize_kenny_micro_roguelike_world();
        // Initialize_kenny_micro_roguelike_characters();
        Initialize_kenny_tiny_dungeon_world();
        Initialize_kenny_tiny_dungeon_characters();
    }

    void Initialize_kenny_tiny_dungeon_world()
    {
        RegisterTile(0, "Sand_clean", 0, 4, true, 2);
        RegisterTile(1, "Sand_gravel", 1, 4, true, 2);
        RegisterTile(2, "Sand_pebbles", 6, 3, true, 2);

        RegisterTile(10, "Wall_Top", 2, 0, false, 2);
        RegisterTile(11, "Wall_Left", 1, 1, false, 2);
        RegisterTile(12, "Wall_Right", 3, 1, false, 2);
        RegisterTile(13, "Wall_Bottom", 2, 2, false, 2);

        // CORNERS
        RegisterTile(14, "Corner_TL", 1, 0, false, 2);
        RegisterTile(15, "Corner_TR", 3, 0, false, 2);
        RegisterTile(16, "Corner_BL", 1, 2, false, 2);
        RegisterTile(17, "Corner_BR", 3, 2, false, 2);

        // solo pillar
        RegisterTile(18, "Pillar", 5, 3, false, 2);
        // VOID/FILLER
        RegisterTile(99, "Void", 1, 1, false, 2);

        // bitmask NWES : 8 4 2 1
        bitmaskMap[12] = 14; // N + W
        bitmaskMap[10] = 15; // N + E
        bitmaskMap[5] = 16;  // W + S
        bitmaskMap[3] = 17;  // W + E

        bitmaskMap[6] = 99; // N + S (Default to void)
        bitmaskMap[9] = 99; // W + E (Default to void)
        bitmaskMap[15] = 99; // N + W + E + S (Roof)
        bitmaskMap[0] = 18;  // No neighbors (Isolated Pillar)
    }

    void Initialize_kenny_tiny_dungeon_characters()
    {
        RegisterTile(20, "Hero", 1, 7, false, 2);
        // assign all fake 4,10 to rat monster for now
        RegisterTile(21, "Skeleton_Warrior", 4, 10, false, 2);
        RegisterTile(22, "Orc_Grunt", 4, 10, false, 2);
        RegisterTile(23, "Crab", 4, 10, false, 2);
        RegisterTile(24, "Floating_Eye", 4, 10, false, 2);
        RegisterTile(25, "Swamp_Snake", 4, 10, false, 2);
        RegisterTile(26, "Wild_Dog", 4, 10, false, 2);
        RegisterTile(27, "Cave_Rat", 4, 10, false, 2);
        RegisterTile(28, "Wood_Bug", 4, 10, false, 2);
        RegisterTile(29, "Slime_Phantom", 4, 10, false, 2);
        RegisterTile(30, "Ghost", 4, 10, false, 2);
        RegisterTile(31, "Tortoise", 4, 10, false, 2);
        RegisterTile(32, "Octopus", 4, 10, false, 2);
        RegisterTile(33, "Screaming_Mandrake", 4, 10, false, 2);
    }

    void Initialize_kenny_micro_roguelike_world()
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

        // bitmask NWES : 8 4 2 1
        bitmaskMap[12] = 14; // N + W
        bitmaskMap[10] = 15; // N + E
        bitmaskMap[5] = 16;  // W + S
        bitmaskMap[3] = 17;  // W + E

        bitmaskMap[6] = 10; // N + S (Default to Top Wall)
        bitmaskMap[9] = 11; // W + E (Default to Left Wall)
    }

    void Initialize_kenny_micro_roguelike_characters()
    {
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
        return 99; // Default to void if unknown
    }

private:
    std::unordered_map<int, TileData> registry;

    // UPDATED: Now takes (gridX, gridY) instead of SDL_Rect
    void RegisterTile(int id, std::string name, int gridX, int gridY, bool walkable, int atlasSizeMultiplier = 1)
    {
        TileData data;
        data.id = id;
        data.name = name;
        data.isWalkable = walkable;

        // AUTOMATIC MATH: Calculate pixels here so you don't have to!
        data.srcRect.x = gridX * ATLAS_TILE_SIZE * atlasSizeMultiplier;
        data.srcRect.y = gridY * ATLAS_TILE_SIZE * atlasSizeMultiplier;
        data.srcRect.w = ATLAS_TILE_SIZE * atlasSizeMultiplier;
        data.srcRect.h = ATLAS_TILE_SIZE * atlasSizeMultiplier;

        registry[id] = data;
    }

    std::unordered_map<int, int> bitmaskMap;

    AssetRegistry() {}
};