#pragma once
#include <vector>
#include <algorithm>
#include "Entity.h"
#include "InputManager.h"
#include "TileMap.h"

class EntityManager
{
public:
    // Singleton Access
    static EntityManager &Get()
    {
        static EntityManager instance;
        return instance;
    }

    // --- SERVICE METHODS (The "Enquiry" Part) ---

    // Efficiently checks if ANYONE is at this specific tile
    Entity *GetEntityAt(int x, int y)
    {
        // Optimization: For now loop is fine, later we can use a 2D Array/Map for O(1) lookup
        for (Entity *e : entities)
        {
            if (e->IsAlive() && e->GetPos().x == x && e->GetPos().y == y)
            {
                return e;
            }
        }
        return nullptr;
    }

    // --- MANAGEMENT ---
    void AddEntity(Entity *e)
    {
        entities.push_back(e);
    }

    void UpdateAll(float dt, InputManager *input, TileMap *map)
    {
        for (Entity *e : entities)
        {
            if (e->IsAlive())
            {
                // Notice: We don't pass 'others' anymore!
                // Entities will call EntityManager::Get() internally if they need to.
                e->Update(dt, input, map);
            }
        }
    }

    void RenderAll(SDL_Renderer *renderer, SDL_Texture *texture, SDL_Rect camera)
    {
        for (Entity *e : entities)
        {
            e->Render(renderer, texture, camera);
        }
    }

    void Cleanup()
    {
        // Remove dead bodies
        auto it = std::remove_if(entities.begin(), entities.end(),
                                 [](Entity *e)
                                 {
                                     if (!e->IsAlive())
                                     {
                                         delete e; // Free memory
                                         return true;
                                     }
                                     return false;
                                 });
        entities.erase(it, entities.end());
    }

    void ClearAll()
    {
        for (Entity *e : entities)
            delete e;
        entities.clear();
    }

private:
    std::vector<Entity *> entities;

    // Private Constructor (Singleton)
    EntityManager() {}
};